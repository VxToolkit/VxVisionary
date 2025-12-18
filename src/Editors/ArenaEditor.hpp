//
// Created by Marco Stulic on 12/18/25.
//

#pragma once
#include <qtmetamacros.h>

#include "vxEditor.hpp"

class ArenaEditor : public vxEditor {
    Q_OBJECT
public:
    virtual ~ArenaEditor();
    ArenaEditor(QObject* parent, QQmlApplicationEngine* engine, QObject* window);
    EditorType type() const override;
};
