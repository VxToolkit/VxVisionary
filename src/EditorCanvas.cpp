//
// Created by Marco Stulic on 1/21/26.
//

#include "EditorCanvas.hpp"
#include <QPainter>
#include <utility>
#include <QRectF>
#include <algorithm>

EditorCanvas::EditorCanvas(QQuickItem* parent) : QQuickPaintedItem(parent) {
    setImplicitWidth(600);
    setImplicitHeight(600);

    canvasOffset = {0.0, 0.0};
    canvasScale = CANVAS_PPI; // 100 pixels per inch
}

void EditorCanvas::paint(QPainter* painter) {
    painter->fillRect(boundingRect(), QColor(16, 16, 16));
    painter->setRenderHint(QPainter::Antialiasing);

    painter->translate(width() / 2.0, height() / 2.0);
    painter->scale(canvasScale, canvasScale);

    painter->translate(canvasOffset.x, canvasOffset.y);
    if (drawableProvider) {
        auto drawables = drawableProvider();
        for (auto& drawable : drawables) {
            drawable->draw(painter);
        }
    }
}

void EditorCanvas::setDrawableProvider(DrawableProviderFunc func) {
    qDebug() << "drawable provider registered!";
    drawableProvider = std::move(func);
}

std::vector<CanvasDrawable*> EditorCanvas::getDrawables() const {
    if (drawableProvider) {
        return drawableProvider();
    }
    return {};
}

Vec2 EditorCanvas::getCanvasOffset() const {
    return canvasOffset;
}

double EditorCanvas::getCanvasScale() const {
    return canvasScale;
}

void EditorCanvas::setCanvasOffset(const Vec2& offset) {
    canvasOffset = offset;
}

void EditorCanvas::setCanvasScale(double scale) {
    canvasScale = scale;
}

void EditorCanvas::fitToView() {
    auto drawables = getDrawables();

    if (drawables.empty()) {
        canvasScale = CANVAS_PPI;
        canvasOffset = {0.0, 0.0};
        update();
        return;
    }

    QRectF boundingBox;
    bool first = true;

    for (auto drawable : drawables) {
        QRectF bounds = drawable->getBounds();
        if (!bounds.isEmpty()) {
            if (first) {
                boundingBox = bounds;
                first = false;
            } else {
                boundingBox = boundingBox.united(bounds);
            }
        }
    }

    if (boundingBox.isEmpty()) {
        canvasScale = CANVAS_PPI;
        canvasOffset = {0.0, 0.0};
        update();
        return;
    }
    double canvasWidth = width() / CANVAS_PPI;
    double canvasHeight = height() / CANVAS_PPI;

    const double padding = 0.1;
    double contentWidth = boundingBox.width() * (1.0 + padding * 2);
    double contentHeight = boundingBox.height() * (1.0 + padding * 2);

    double scaleX = canvasWidth / contentWidth;
    double scaleY = canvasHeight / contentHeight;
    double newScale = std::min(scaleX, scaleY) * CANVAS_PPI;
    newScale = std::max(newScale, 1.0); // Ensure minimum scale of 1

    canvasScale = newScale;

    double centerX = boundingBox.center().x();
    double centerY = boundingBox.center().y();
    canvasOffset = {-centerX, -centerY};

    update();
}
