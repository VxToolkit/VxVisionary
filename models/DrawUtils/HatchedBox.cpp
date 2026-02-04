//
// Created by Marco Stulic on 2/3/26.
//

#include "HatchedBox.hpp"

void drawHatchedBox(QPainter* painter, const QRectF& rect, const BoxStyle& style) {
    painter->save();

    painter->setOpacity(style.opacity);

    QBrush hatchBrush(style.baseColor, style.pattern);
    QTransform deviceTransform = painter->deviceTransform();
    QTransform brushTransform;

    if (!qFuzzyIsNull(deviceTransform.m11()) && !qFuzzyIsNull(deviceTransform.m22())) {
        brushTransform.scale(1.0 / deviceTransform.m11(), 1.0 / deviceTransform.m22());
    }
    hatchBrush.setTransform(brushTransform);

    painter->setPen(Qt::NoPen);
    painter->setBrush(hatchBrush);
    painter->drawRect(rect);

    qreal radius = qMax(rect.width(), rect.height()) * 0.5;
    QRadialGradient gradient(rect.center(), radius);

    gradient.setColorAt(0.0, Qt::transparent);
    gradient.setColorAt(0.8, style.glowColor);

    QColor edgeColor = style.glowColor;
    edgeColor.setAlpha(qMin(255, edgeColor.alpha() + 50));
    gradient.setColorAt(1.0, edgeColor);

    painter->setBrush(gradient);
    painter->drawRect(rect);

    QPen borderPen(style.baseColor);
    borderPen.setWidthF(style.borderWidth);
    borderPen.setCosmetic(true);
    borderPen.setStyle(style.lineStyle);

    painter->setPen(borderPen);
    painter->setBrush(Qt::NoBrush);
    painter->drawRect(rect);

    painter->restore();
}