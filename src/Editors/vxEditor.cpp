//
// Created by user on 12/17/25.
//

#include "vxEditor.hpp"

#include <QQmlApplicationEngine>

vxEditor::vxEditor(QObject* parent, QQmlApplicationEngine* engine, QObject* window)
    : QObject(parent), m_engine(engine), m_window(window) {}

EditorType vxEditor::type() const {
    return EditorType::Null;
}