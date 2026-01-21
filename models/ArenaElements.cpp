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