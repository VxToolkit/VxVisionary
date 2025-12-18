//
// Created by user on 12/17/25.
//

#pragma once
#include "QDataStream"
#include "primitives.hpp"

class CopyUtils {
public:
    static void writeVec3(const Vec3& vec, QDataStream& stream);
    static Vec3 readVec3(QDataStream& stream);

    static void writeVec2(const Vec2& vec, QDataStream& stream);
    static Vec2 readVec2(QDataStream& stream);

    static void writeTransforms2D(const Transforms2D& transforms, QDataStream& stream);
    static Transforms2D readTransforms2D(QDataStream& stream);
};