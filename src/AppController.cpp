#include "AppController.h"

#include <QQmlApplicationEngine>
#include <qqmlcontext.h>
#include <QFileDialog>
#include <QFile>

#include "QMessageBox"
using namespace Qt::StringLiterals;

AppController::AppController(QQmlApplicationEngine* engine, QObject* parent) : QObject(parent) , m_statusMessage("Ready") , m_engine(engine) {
    m_engine->rootContext()->setContextProperty("appController", this);
    const QUrl mainQML(u"qrc:/qt/qml/Main/content/Main.qml"_s);
    m_engine->load(mainQML);
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