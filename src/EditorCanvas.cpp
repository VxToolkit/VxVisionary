//
// Created by Marco Stulic on 1/21/26.
//

#include "EditorCanvas.hpp"
#include <QPainter>
#include <utility>

EditorCanvas::EditorCanvas(QQuickItem* parent) : QQuickPaintedItem(parent) {}

void EditorCanvas::paint(QPainter* painter) {
    painter->setRenderHint(QPainter::Antialiasing);
    if (drawableProvider) {
        auto drawables = drawableProvider();
        for (auto& drawable : drawables) {
            qDebug () << "Drawing drawable";
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
