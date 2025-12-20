//
// Created by user on 12/17/25.
//

#pragma once
#include <QQmlApplicationEngine>
#include <QString>
#include <qtmetamacros.h>

class AppController;
#include "models/Project.hpp"

enum class EditorType {
    Null,
    Arena,
    Mission
};

class vxEditor : public QObject {
    Q_OBJECT
public:
    vxEditor(QObject* parent, QQmlApplicationEngine* engine, QObject* window, AppController* controller);

    virtual EditorType type() const;
    virtual void assetRecieved(Asset* asset);

protected:
    AppController* m_controller;
    QQmlApplicationEngine* m_engine = nullptr;
    QObject* m_window = nullptr;
};
