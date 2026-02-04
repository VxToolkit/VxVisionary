//
// Created by Marco Stulic on 2/3/26.
//
#pragma once

#include <QPainter>
#include <QBrush>
#include <QPen>
#include <QTransform>
#include <QtMath>

// 1. Configuration struct to keep arguments clean
struct BoxStyle {
    QColor baseColor;         // Used for hatch and border
    QColor glowColor;         // Used for the gradient edges
    Qt::BrushStyle pattern;   // The hatch pattern
    Qt::PenStyle lineStyle;   // Solid vs Dash
    qreal borderWidth{};
    qreal opacity{};            // New opacity property
};

void drawHatchedBox(QPainter* painter, const QRectF& rect, const BoxStyle& style);