//
// Created by Marco Stulic on 1/21/26.
//

#include "EditorCanvas.hpp"
#include <QPainter>
#include <utility>
#include <QRectF>
#include <algorithm>
#include <QMouseEvent>
#include <QWheelEvent>
#include <QNativeGestureEvent>

EditorCanvas::EditorCanvas(QQuickItem* parent) : QQuickPaintedItem(parent) {
    setImplicitWidth(600);
    setImplicitHeight(600);

    canvasOffset = {0.0, 0.0};
    canvasScale = CANVAS_PPI; // 100 pixels per inch

    isPanning = false;

    setAcceptedMouseButtons(Qt::LeftButton | Qt::MiddleButton | Qt::RightButton);
    setAcceptHoverEvents(true);
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

void EditorCanvas::mousePressEvent(QMouseEvent* event) {
    if (event->button() == Qt::MiddleButton ||
        (event->button() == Qt::LeftButton && event->modifiers() & Qt::ShiftModifier)) {
        isPanning = true;
        lastMousePos = event->pos();
        event->accept();
    } else {
        QQuickPaintedItem::mousePressEvent(event);
    }
}

void EditorCanvas::mouseMoveEvent(QMouseEvent* event) {
    if (isPanning) {
        QPointF delta = event->pos() - lastMousePos;

        canvasOffset.x += delta.x() / canvasScale;
        canvasOffset.y += delta.y() / canvasScale;

        lastMousePos = event->pos();
        update();
        event->accept();
    } else {
        QQuickPaintedItem::mouseMoveEvent(event);
    }
}

void EditorCanvas::mouseReleaseEvent(QMouseEvent* event) {
    if (event->button() == Qt::MiddleButton ||
        (event->button() == Qt::LeftButton && isPanning)) {
        isPanning = false;
        event->accept();
    } else {
        QQuickPaintedItem::mouseReleaseEvent(event);
    }
}

void EditorCanvas::wheelEvent(QWheelEvent* event) {
    QPoint numPixels = event->pixelDelta();
    QPoint numDegrees = event->angleDelta();

    if (!numPixels.isNull()) {
        canvasOffset.x += numPixels.x() / canvasScale;
        canvasOffset.y += numPixels.y() / canvasScale;
        update();
    } else if (!numDegrees.isNull()) {
        if (event->modifiers() & Qt::ControlModifier) {
            double delta = numDegrees.y() / 120.0;
            double factor = qPow(ZOOM_FACTOR, delta);
            zoomAt(event->position(), factor);
        } else {
            canvasOffset.x += numDegrees.x() / 120.0 * 20.0 / canvasScale;
            canvasOffset.y += numDegrees.y() / 120.0 * 20.0 / canvasScale;
            update();
        }
    }

    event->accept();
}

bool EditorCanvas::event(QEvent* event) {
    if (event->type() == QEvent::NativeGesture) {
        auto* gestureEvent = static_cast<QNativeGestureEvent*>(event);
        switch (gestureEvent->gestureType()) {
            case Qt::ZoomNativeGesture: {
                double zoomValue = gestureEvent->value();
                double factor = 1.0 + zoomValue;
                zoomAt(gestureEvent->position(), factor);
                return true;
            }
            case Qt::SmartZoomNativeGesture: {
                fitToView();
                return true;
            }
            default:
                break;
        }
    }
    return QQuickPaintedItem::event(event);
}

void EditorCanvas::zoomAt(const QPointF& pos, double factor) {
    double oldScale = canvasScale;
    double newScale = oldScale * factor;

    newScale = qMax(MIN_SCALE, qMin(MAX_SCALE, newScale));

    if (qFuzzyCompare(oldScale, newScale)) {
        return;
    }

    double worldX = (pos.x() - width() / 2.0) / oldScale - canvasOffset.x;
    double worldY = (pos.y() - height() / 2.0) / oldScale - canvasOffset.y;

    canvasScale = newScale;

    canvasOffset.x = (pos.x() - width() / 2.0) / newScale - worldX;
    canvasOffset.y = (pos.y() - height() / 2.0) / newScale - worldY;

    update();
}

