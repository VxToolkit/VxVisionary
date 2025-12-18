//
// Created by Marco Stulic on 12/18/25.
//

#include "ArenaEditor.hpp"

#include <QQuickWindow>

#include "AppController.h"

EditorType ArenaEditor::type() const {
    return EditorType::Arena;
}

ArenaEditor::ArenaEditor(QObject* parent, QQmlApplicationEngine* engine, QObject* window) : vxEditor(parent, engine, window) {
    m_window->setProperty("visible", true);
    QQuickWindow *quickWindow = qobject_cast<QQuickWindow*>(m_window);
    connect(quickWindow, &QQuickWindow::closing, this, []() {
        AppController::deleteEditorPtr(EditorType::Arena);
    });
}

ArenaEditor::~ArenaEditor() {
    m_window->setProperty("visible", false);
}