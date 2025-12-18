//
// Created by user on 12/17/25.
//

#include "CopyUtils.hpp"

void CopyUtils::writeVec3(const Vec3& vec, QDataStream& stream) {
    stream << vec.x;
    stream << vec.y;
    stream << vec.z;
}

Vec3 CopyUtils::readVec3(QDataStream& stream) {
    Vec3 vec(0,0,0);
    stream >> vec.x;
    stream >> vec.y;
    stream >> vec.z;
    return vec;
}

void CopyUtils::writeVec2(const Vec2& vec, QDataStream& stream) {
    stream << vec.x;
    stream << vec.y;
}

Vec2 CopyUtils::readVec2(QDataStream& stream) {
    Vec2 vec(0,0);
    stream >> vec.x;
    stream >> vec.y;
    return vec;
}

void CopyUtils::writeTransforms2D(const Transforms2D& transforms, QDataStream& stream) {
    stream << transforms.position.x;
    stream << transforms.position.y;
    stream << transforms.scale.x;
    stream << transforms.scale.y;
}

Transforms2D CopyUtils::readTransforms2D(QDataStream& stream) {
    Transforms2D transforms({0,0}, {0,0});
    stream >> transforms.position.x;
    stream >> transforms.position.y;
    stream >> transforms.scale.x;
    stream >> transforms.scale.y;
    return transforms;
}
