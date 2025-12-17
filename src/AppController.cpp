#include "AppController.h"

ProjectModel AppController::m_projectModel;

AppController::AppController(QObject* parent)
    : QObject(parent)
    , m_statusMessage("Ready") {
}

ProjectModel* AppController::projectModel() {
    return const_cast<ProjectModel*>(&m_projectModel);
}