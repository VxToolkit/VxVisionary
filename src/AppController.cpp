#include "AppController.h"

AppController::AppController(QObject* parent)
    : QObject(parent)
    , m_statusMessage("Ready") {
}

QString AppController::statusMessage() const {
    return m_statusMessage;
}

void AppController::setStatusMessage(const QString& message) {
    if (m_statusMessage != message) {
        m_statusMessage = message;
        emit statusMessageChanged();
    }
}

void AppController::performAction() {
    setStatusMessage("Action performed!");
}

