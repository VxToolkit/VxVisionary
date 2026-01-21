//
// Created by Marco Stulic on 1/21/26.
//

#pragma once
#include <QPainter>

class CanvasDrawable {
public:
    virtual ~CanvasDrawable() = default;
    virtual void draw(QPainter* painter);
};