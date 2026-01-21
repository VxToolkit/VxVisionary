//
// Created by user on 12/17/25.
//

#pragma once
#include <any>
#include <ostream>
#include <QDataStream>

#include "primitives.hpp"
#include <QString>

#include "ArenaElementRegistry.hpp"
#include "CanvasDrawable.hpp"

enum class ArenaElementType {
    Null,
    NoGo,
    Goal
};

using ArenaElementProps = std::vector<std::any>;
using ArenaElementNames = std::vector<QString>;

class ArenaElement : public virtual CanvasDrawable {
public:
    virtual ~ArenaElement() = default;
    [[nodiscard]] virtual ArenaElementType getType() const;
    virtual void outputData(QDataStream& stream) const;
    virtual void inputData(QDataStream& stream);

    virtual ArenaElementProps getProperties() const;
    virtual ArenaElementNames getPropertyNames() const;
    virtual void setProperty(int idx, const std::any& value);

    QString name;
};

class NoGoElement : public ArenaElement {
public:
    [[nodiscard]] ArenaElementType getType() const override;

    ~NoGoElement() override = default;
    Transforms2D transforms;

    void outputData(QDataStream& stream) const override;
    void inputData(QDataStream& stream) override;

    ArenaElementProps getProperties() const override;
    ArenaElementNames getPropertyNames() const override;

    void setProperty(int idx, const std::any& value) override;

    void draw(QPainter* painter) override;
};

class GoalElement : public ArenaElement {
public:
    [[nodiscard]] ArenaElementType getType() const override;
    ~GoalElement() override = default;

    Vec2 position;

    void outputData(QDataStream& stream) const override;
    void inputData(QDataStream& stream) override;

    ArenaElementProps getProperties() const override;
    ArenaElementNames getPropertyNames() const override;

    void setProperty(int idx, const std::any& value) override;
    void draw(QPainter* painter) override;
};

REGISTER_ARENA_ELEMENT("No-Go Zone",NoGoElement)
REGISTER_ARENA_ELEMENT("Goal Element",GoalElement)