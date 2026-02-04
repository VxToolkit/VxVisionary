//
// Created by Marco Stulic on 1/23/26.
//

#include "ArenaDrawable.hpp"

#include "models/ArenaAsset.hpp"

void ArenaDrawable::draw(QPainter* painter) {
    if (!linkedAsset) return;

    painter->save();

    float w = linkedAsset->getBounds().x;
    float h = linkedAsset->getBounds().y;

    QRectF arenaRect(-w / 2.0, -h / 2.0, w, h);

    QColor hatchColor(60, 55, 76, 180);
    QColor borderColor(80, 75, 150);

    QBrush hatchBrush(hatchColor, Qt::DiagCrossPattern);
    QTransform deviceTransform = painter->deviceTransform();
    QTransform brushTransform;
    brushTransform.scale(1.0 / deviceTransform.m11(), 1.0 / deviceTransform.m22());
    hatchBrush.setTransform(brushTransform);

    painter->setBrush(hatchBrush);
    painter->setPen(Qt::NoPen);
    painter->drawRect(arenaRect);

    QRadialGradient fadeGradiant(0, 0, qMax(w, h) * 0.5);
    fadeGradiant.setColorAt(0.0, Qt::transparent);
    fadeGradiant.setColorAt(0.9, QColor(16, 16, 16, 50));
    fadeGradiant.setColorAt(1.0, QColor(16, 16, 16, 100));

    painter->setBrush(fadeGradiant);
    painter->drawRect(arenaRect);

    QPen borderPen(borderColor);
    borderPen.setWidthF(2.0);
    borderPen.setCosmetic(true);
    borderPen.setStyle(Qt::DashLine);

    painter->setPen(borderPen);
    painter->setBrush(Qt::NoBrush);
    painter->drawRect(arenaRect);

    painter->restore();
}

void ArenaDrawable::setLinkedAsset(ArenaAsset* asset) {
    linkedAsset = asset;
}