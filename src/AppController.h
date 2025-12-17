#pragma once

#include <QObject>
#include <QString>
#include <QtQml/qqml.h>
#include <QQmlApplicationEngine>

#include "models/Project.hpp"
#include "models/ProjectSelector/ProjectModel.hpp"

class AppController : public QObject {
    Q_OBJECT
    Q_PROPERTY(ProjectModel* projectModel READ projectModel CONSTANT)
    Q_PROPERTY(Project* currentLoadedProject READ currentLoadedProject CONSTANT)
    Q_PROPERTY(QString projectName READ getProjectName CONSTANT)

public:
    explicit AppController(QQmlApplicationEngine* engine, QObject* parent = nullptr);
    ProjectModel* projectModel();
    Project *currentLoadedProject();
    QString getProjectName() const;
    Q_INVOKABLE void loadProject(const QString& projectPath);
    Q_INVOKABLE void createNewProject();

private:
    QString m_statusMessage;
    ProjectModel m_projectModel;
    QQmlApplicationEngine* m_engine = nullptr;
    Project* m_currentLoadedProject = nullptr;
};
