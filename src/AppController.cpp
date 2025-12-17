#include "AppController.h"

AppController::AppController(QObject* parent)
    : QObject(parent)
    , m_statusMessage("Ready") {
}

ProjectModel* AppController::projectModel() const {
    return const_cast<ProjectModel*>(&m_projectModel);
}