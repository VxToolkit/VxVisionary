//
// Created by Marco Stulic on 1/21/26.
//

#include "EditorCanvas.hpp"
#include <QPainter>

EditorCanvas::EditorCanvas(QQuickItem* parent) : QQuickPaintedItem(parent) {}

void EditorCanvas::paint(QPainter* painter) {
    painter->setRenderHint(QPainter::Antialiasing);
    painter->setBrush(Qt::blue);
    painter->drawRect(boundingRect());
}