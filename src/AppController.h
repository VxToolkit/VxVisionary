#pragma once

#include <QObject>
#include <QString>
#include <QtQml/qqml.h>
#include "models/ProjectSelector/ProjectModel.hpp"

class AppController : public QObject {
    Q_OBJECT
    Q_PROPERTY(ProjectModel* projectModel READ projectModel CONSTANT)
    QML_ELEMENT

public:
    explicit AppController(QObject* parent = nullptr);

    static ProjectModel* projectModel();

private:
    QString m_statusMessage;
    static ProjectModel m_projectModel;
};
