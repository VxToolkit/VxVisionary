#include "AppController.h"

#include <QQmlApplicationEngine>
#include <qqmlcontext.h>
#include <QFileDialog>
#include <QFile>
#include <qqmlcomponent.h>
#include <QQuickWindow>

#include "Editors/vxEditor.hpp"

#include "QMessageBox"
#include "Editors/ArenaEditor.hpp"
#include "models/Assets/AssetFilterModel.hpp"

using namespace Qt::StringLiterals;

std::vector<vxEditor*> AppController::m_openEditors;
std::unordered_map<EditorType, QObject*> AppController::m_editorWindows;

AppController::AppController(QQmlApplicationEngine* engine, QObject* parent) : QObject(parent) , m_statusMessage("Ready") , m_engine(engine) {
    m_engine->rootContext()->setContextProperty("appController", this);
    qmlRegisterType<AssetFilterModel>("VxtVisionary.Models", 1, 0, "AssetFilterModel");

    qmlRegisterUncreatableMetaObject(
    Vxt::staticMetaObject,
    "VxtVisionary.Models",
    1, 0,
    "AssetType",
    "Error: Cannot create enum"
    );

    const QUrl mainQML(u"qrc:/qt/qml/Main/content/Main.qml"_s);
    m_engine->load(mainQML);

    QQmlComponent arenaEditorComponent(m_engine, QUrl(QStringLiteral("qrc:/qt/qml/Main/content/ArenaEditor.qml")));

    m_assetModel = new AssetModel(this, this);
    setAssetModel(m_assetModel);

    QObject* arenaEditorWindow = arenaEditorComponent.create();

    if (!arenaEditorWindow) {
        qWarning() << "Error creating window:" << arenaEditorComponent.errorString();
        return;
    }

    arenaEditorWindow->setProperty("visible", false);
    m_editorWindows[EditorType::Arena] = arenaEditorWindow;
}

ProjectModel* AppController::projectModel() {
    return const_cast<ProjectModel*>(&m_projectModel);
}

Project *AppController::currentLoadedProject() {
    return m_currentLoadedProject;
}

void AppController::loadProject(const QString &projectPath) {
    try {
        Project* newProject = new Project(projectPath);

        delete m_currentLoadedProject;

        m_currentLoadedProject = newProject;

        Project::addRecentProject(projectPath.toStdString());

        QUrl dashboardQML(QStringLiteral("qrc:/qt/qml/Main/content/ProjectDashboard.qml"));
        m_engine->load(dashboardQML);

        QObject *found_object = m_engine->rootObjects().first();
        QMetaObject::invokeMethod(found_object, "close");

        QObject *newDashboard = m_engine->rootObjects().last();
        QQuickWindow *newWindow = qobject_cast<QQuickWindow*>(newDashboard);

        if (newWindow) {
            connect(newWindow, &QQuickWindow::closing, [this]() {
                delete m_currentLoadedProject;
                m_currentLoadedProject = nullptr;
                QCoreApplication::quit();
            });
        }

        emit projectNameChanged();

    } catch (const std::runtime_error& e) {
        QMessageBox::critical(nullptr, "Could not load project file", e.what());
    }
}

void AppController::createNewProject() {
    QString fileName = QFileDialog::getSaveFileName(nullptr, "Create New Project", QDir::homePath(), "VxTemplate Project Files (*.vxtemplate)");
    if (fileName.isEmpty()) {
        QMessageBox::critical(nullptr, "Could not create project file", "No file name was specified.");
        return;
    }
    QFileInfo info(fileName);
    QString projectName = info.baseName();

    Project* newProject = new Project(projectName, info.absolutePath());
    Project::addRecentProject(fileName.toStdString());

    newProject->save();
    delete newProject;
    loadProject(fileName);
}

QString AppController::getProjectName() const {
    if (m_currentLoadedProject) {
        return m_currentLoadedProject->getName();
    }
    return "null";
}

void AppController::openWorkspace(QString name) {
    if (name == "Arena Editor") {
        ArenaEditor* editorFound = getEditorOfType<ArenaEditor>(EditorType::Arena);
        if (editorFound) {
            QMessageBox::critical(nullptr, "Could not open workspace.", "An arena editor window is already open.");
            return;
        }
        QObject *window = m_editorWindows[EditorType::Arena];
        if (window == nullptr) {
            QMessageBox::critical(nullptr, "Could not open workspace.", "The arena editor window could not be found.");
            return;
        }

        ArenaEditor* arenaEditor = new ArenaEditor(this, m_engine, window, this);
        m_openEditors.push_back(arenaEditor);
    }
}

QObject* AppController::getEditorWindow(EditorType type) {
    for (vxEditor* editor : m_openEditors) {
        if (editor->type() == type) {
            return editor;
        }
    }
    return nullptr;
}

void AppController::deleteEditorPtr(EditorType type) {
    m_openEditors.erase(std::remove_if(m_openEditors.begin(), m_openEditors.end(),
        [type](vxEditor* editor) {
            if (editor->type() == type) {
                delete editor;
                qWarning() << "cleaned editor type" << static_cast<int>(type);
                return true;
            }
            return false;
        }), m_openEditors.end());
}

std::vector<Asset*> AppController::getAssetsOfType(Vxt::AssetType type) const {
    if (m_currentLoadedProject) {
        return m_currentLoadedProject->getAssetsOfType(type);
    }
    return {};
}

void AppController::saveCurrentProject() {
    if (m_currentLoadedProject) {
        m_currentLoadedProject->save();
    }
}

AssetModel* AppController::assetModel() const {
    return m_assetModel;
}

void AppController::setAssetModel(AssetModel* model) {
    m_assetModel = model;
    emit assetModelChanged();
}

std::vector<Asset*>& AppController::getAssets() const {
    if (m_currentLoadedProject) {
        return m_currentLoadedProject->getAssets();
    }
    static std::vector<Asset*> empty;
    return empty;
}

void AppController::askForAsset(vxEditor* editor, Vxt::AssetType type) {
    if (m_editorAskedForAsset) {
        QMessageBox::critical(nullptr, "Error", "An editor is already requesting an asset.");
        return;
    }
    m_editorAskedForAsset = editor;
    emit requestOpenAssetDiag(static_cast<qint32>(type));
}

void AppController::assetReceived(QString name) {
    if (!m_editorAskedForAsset) {
        QMessageBox::critical(nullptr, "Error", "No editor is requesting an asset.");
        return;
    }

    Asset* foundAsset = nullptr;
    for (Asset* asset : getAssets()) {
        if (asset->getName() == name) {
            foundAsset = asset;
            break;
        }
    }

    if (!foundAsset) {
        QMessageBox::critical(nullptr, "Error", "The requested asset could not be found.");
        m_editorAskedForAsset = nullptr;
        return;
    }

    m_editorAskedForAsset->assetRecieved(foundAsset);
    m_editorAskedForAsset = nullptr;
}

void AppController::askForGenericElement(vxEditor* editor, std::function<void(QString)> callback, QString title, QStringList data) {
    if (m_genericElementReceivedCallback != nullptr) {
        QMessageBox::critical(nullptr, "Error", "An editor is already requesting a generic element.");
        return;
    }
    m_genericElementReceivedCallback = callback;
    emit requestGenericElementPicker(title, data);
}

void AppController::genericElementReceived(QString picker) {
    if (!m_genericElementReceivedCallback) {
        QMessageBox::critical(nullptr, "Error", "No editor is requesting a generic element.");
        return;
    }

    m_genericElementReceivedCallback(std::move(picker));
    m_genericElementReceivedCallback = nullptr;
}