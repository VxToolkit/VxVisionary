//
// Created by user on 12/17/25.
//

#include "ArenaElements.hpp"

#include "ArenaAsset.hpp"
#include "Asset.hpp"
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
    stream << static_cast<int>(getType());
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

ArenaElementType GoalElement::getType() const {
    return ArenaElementType::Goal;
}

