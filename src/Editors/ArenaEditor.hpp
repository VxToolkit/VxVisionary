//
// Created by Marco Stulic on 12/18/25.
//

#pragma once
#include <qtmetamacros.h>

#include "AppController.h"
#include "vxEditor.hpp"

class ArenaEditor : public vxEditor {
    Q_OBJECT
public:
    virtual ~ArenaEditor();
    ArenaEditor(QObject* parent, QQmlApplicationEngine* engine, QObject* window, AppController* controller);

    Q_INVOKABLE void newArena() const;
    EditorType type() const override;
};
