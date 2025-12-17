#pragma once

#include <QObject>
#include <QString>
#include <QtQml/qqml.h>
#include "ProjectSelector/ProjectModel.hpp"

class AppController : public QObject {
    Q_OBJECT
    Q_PROPERTY(ProjectModel* projectModel READ projectModel CONSTANT)
    QML_ELEMENT

public:
    explicit AppController(QObject* parent = nullptr);

    ProjectModel* projectModel() const;

private:
    QString m_statusMessage;
    ProjectModel m_projectModel;
};
