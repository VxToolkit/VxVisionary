//
// Created by user on 12/17/25.
//

#pragma once
#include <ostream>
#include <QDataStream>

#include "primitives.hpp"
#include <QString>

#include "ArenaElementRegistry.hpp"

enum class ArenaElementType {
    Null,
    NoGo,
    Goal
};

class ArenaElement {
public:
    virtual ~ArenaElement() = default;
    [[nodiscard]] virtual ArenaElementType getType() const;
    virtual void outputData(QDataStream& stream) const;
    virtual void inputData(QDataStream& stream);

    QString name;
};

class NoGoElement : public ArenaElement {
public:
    [[nodiscard]] ArenaElementType getType() const override;

    ~NoGoElement() override = default;
    Transforms2D transforms;

    void outputData(QDataStream& stream) const override;
    void inputData(QDataStream& stream) override;
};

class GoalElement : public ArenaElement {
public:
    [[nodiscard]] ArenaElementType getType() const override;
    ~GoalElement() override = default;

    Vec2 position;

    void outputData(QDataStream& stream) const override;
    void inputData(QDataStream& stream) override;
};

REGISTER_ARENA_ELEMENT("No-Go Zone",NoGoElement)
REGISTER_ARENA_ELEMENT("Goal Element",GoalElement)