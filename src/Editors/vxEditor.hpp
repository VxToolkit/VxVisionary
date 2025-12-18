//
// Created by user on 12/17/25.
//

#pragma once
#include <QQmlApplicationEngine>
#include <QString>
#include <qtmetamacros.h>

#include "models/Project.hpp"

enum class EditorType {
    Null,
    Arena,
    Mission
};

class vxEditor : public QObject {
    Q_OBJECT
public:
    vxEditor(QObject* parent, QQmlApplicationEngine* engine, QObject* window);

    virtual EditorType type() const;

protected:
    QQmlApplicationEngine* m_engine = nullptr;
    QObject* m_window = nullptr;
};
