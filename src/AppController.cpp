#include "AppController.h"

#include <QQmlApplicationEngine>
#include <qqmlcontext.h>
#include <QFileDialog>
#include <QFile>
#include <qqmlcomponent.h>

#include "Editors/vxEditor.hpp"

#include "QMessageBox"
#include "Editors/ArenaEditor.hpp"

using namespace Qt::StringLiterals;

std::vector<vxEditor*> AppController::m_openEditors;
std::unordered_map<EditorType, QObject*> AppController::m_editorWindows;

AppController::AppController(QQmlApplicationEngine* engine, QObject* parent) : QObject(parent) , m_statusMessage("Ready") , m_engine(engine) {
    m_engine->rootContext()->setContextProperty("appController", this);
    const QUrl mainQML(u"qrc:/qt/qml/Main/content/Main.qml"_s);
    m_engine->load(mainQML);

    QQmlComponent arenaEditorComponent(m_engine, QUrl(QStringLiteral("qrc:/qt/qml/Main/content/ArenaEditor.qml")));


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

        ArenaEditor* arenaEditor = new ArenaEditor(this, m_engine, window);
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