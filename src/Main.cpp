#include <QApplication>
#include <QQmlApplicationEngine>
#include <QSplashScreen>
#include <QPixmap>
#include <QQuickWindow>
#include <QTimer>
#include <QDebug>
#include <QDirIterator>
#include <QThread>
#include <QDir>
#include <QStandardPaths>
#include "models/Project.hpp"
#include "AppController.h"

using namespace Qt::StringLiterals;

int main(int argc, char *argv[])
{
    // app
    QApplication app(argc, argv);

    // splashscreen
    QPixmap pixmap(":/qt/qml/Main/assets/VXVisionarySplash.png");
    QPixmap new_pixmap = pixmap.scaled(600*2, 400*2, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    QSplashScreen splash(new_pixmap);
    splash.show();
    app.processEvents();


    // engine
    QQmlApplicationEngine* engine = new QQmlApplicationEngine();


    // load qml
    AppController appController(engine);

    // load project lists
    QString config_path = QStandardPaths::writableLocation(QStandardPaths::AppConfigLocation);

    QDir dir(config_path);
    if (!dir.exists()) {
        dir.mkpath(".");
    }

    Project::getProjectList(dir);
    // Project::addRecentProject("~/Downloads/testProject.vxtemplate");
    std::vector<std::string> projects = Project::getRecentProjects();

    for (const auto& project : projects) {
        appController.projectModel()->addProject(project.c_str());
    }

    // close splashscreen on load
    splash.finish(nullptr);

    QObject::connect(&app, &QCoreApplication::aboutToQuit, [&config_path]() {
       Project::writeRecentProjects(config_path);
    });

    return app.exec();
}