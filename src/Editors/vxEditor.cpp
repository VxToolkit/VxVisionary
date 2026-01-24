//
// Created by user on 12/17/25.
//

#include "vxEditor.hpp"

#include <QQmlApplicationEngine>

vxEditor::vxEditor(QObject* parent, QQmlApplicationEngine* engine, QObject* window, AppController *controller)
    : QObject(parent), m_engine(engine), m_window(window), m_controller(controller) {}

EditorType vxEditor::type() const {
    return EditorType::Null;
}

void vxEditor::assetRecieved(Asset* asset) {
    // default imp
}

void vxEditor::provideWindow(QObject* window) {
    // default imp
}