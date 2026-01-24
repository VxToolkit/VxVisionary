//
// Created by user on 12/17/25.
//

#include "ArenaElements.hpp"

#include "ArenaAsset.hpp"
//#include "Asset.hpp"
#include "CopyUtils.hpp"
#include <QDataStream>
#include "ArenaElementRegistry.hpp"



ArenaElementType NoGoElement::getType() const {
    return ArenaElementType::NoGo;
}

ArenaElementType ArenaElement::getType() const {
    return ArenaElementType::Null;
}


void ArenaElement::outputData(QDataStream& stream) const {
    stream << name;
}

void ArenaElement::inputData(QDataStream& stream) {
    stream >> name;
}

void NoGoElement::outputData(QDataStream& stream) const {
    ArenaElement::outputData(stream);
    CopyUtils::writeTransforms2D(transforms, stream);
}

void NoGoElement::inputData(QDataStream& stream) {
    ArenaElement::inputData(stream);
    transforms = CopyUtils::readTransforms2D(stream);
}

void NoGoElement::draw(QPainter* painter) {
    painter->save();

    painter->translate(transforms.position.x, transforms.position.y);
    painter->scale(transforms.scale.x, transforms.scale.y);

    QRectF rect(-0.5, -0.5, 1.0, 1.0);

    QColor darkRed = QColor(150, 0, 0);
    QBrush hatchBrush(darkRed, Qt::FDiagPattern);
    QTransform deviceTransform = painter->deviceTransform();
    QTransform brushTransform;
    brushTransform.scale(1.0 / deviceTransform.m11(), 1.0 / deviceTransform.m22());
    hatchBrush.setTransform(brushTransform);

    painter->setPen(Qt::NoPen);
    painter->setBrush(hatchBrush);
    painter->drawRect(rect);

    QRadialGradient glowGradient(0, 0, 0.5);
    glowGradient.setColorAt(0.0, Qt::transparent);
    glowGradient.setColorAt(0.8, QColor(255, 0, 0, 40));
    glowGradient.setColorAt(1.0, QColor(255, 0, 0, 120));

    painter->setBrush(glowGradient);
    painter->drawRect(rect);

    QPen borderPen(Qt::red);
    borderPen.setWidthF(1.0);
    borderPen.setCosmetic(true);

    painter->setPen(borderPen);
    painter->setBrush(Qt::NoBrush);
    painter->drawRect(rect);

    painter->restore();
}
void GoalElement::draw(QPainter* painter) {
    painter->save();
    painter->translate(position.x, position.y);

    painter->setBrush(QBrush(Qt::magenta));
    painter->setPen(Qt::NoPen);
    painter->drawEllipse(QPointF(0, 0), 0.25, 0.25);

    painter->restore();
}

void GoalElement::outputData(QDataStream& stream) const {
    ArenaElement::outputData(stream);
    CopyUtils::writeVec2(position, stream);
}

void GoalElement::inputData(QDataStream& stream) {
    ArenaElement::inputData(stream);
    position = CopyUtils::readVec2(stream);
}

ArenaElementProps ArenaElement::getProperties() const {
    return {};
}

ArenaElementNames ArenaElement::getPropertyNames() const {
    return {};
}

void ArenaElement::setProperty(int idx, const std::any& value) {

}

ArenaElementType GoalElement::getType() const {
    return ArenaElementType::Goal;
}

ArenaElementProps NoGoElement::getProperties() const {
    return {transforms.position.x, transforms.position.y, transforms.scale.x, transforms.scale.y};
}

ArenaElementNames NoGoElement::getPropertyNames() const {
    return {"Position X", "Position Y", "Scale X", "Scale Y"};
}

void NoGoElement::setProperty(int idx, const std::any& value) {
    switch (idx) {
        case 0:
            transforms.position.x = std::any_cast<double>(value);
            break;
        case 1:
            transforms.position.y = std::any_cast<double>(value);
            break;
        case 2:
            transforms.scale.x = std::any_cast<double>(value);
            break;
        case 3:
            transforms.scale.y = std::any_cast<double>(value);
            break;
        default:
            break;
    }
}

ArenaElementProps GoalElement::getProperties() const {
    return {position.x, position.y};
}

ArenaElementNames GoalElement::getPropertyNames() const {
    return {"Position X", "Position Y"};
}

void GoalElement::setProperty(int idx, const std::any& value) {
    switch (idx) {
        case 0:
            position.x = std::any_cast<double>(value);
            break;
        case 1:
            position.y = std::any_cast<double>(value);
            break;
        default:
            break;
    }
}