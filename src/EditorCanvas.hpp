//
// Created by Marco Stulic on 1/21/26.
//

#pragma once
#include <QQuickPaintedItem>
#include <CanvasDrawable.hpp>
#include <functional>
#include <vector>
#include <QMetaClassInfo>
#include <qqml.h>
#include <QPointF>

#include "primitives.hpp"

constexpr double CANVAS_PPI = 100.0;
constexpr double MIN_SCALE = 10.0;
constexpr double MAX_SCALE = 500.0;
constexpr double ZOOM_FACTOR = 1.1;

class EditorCanvas : public QQuickPaintedItem {
    Q_OBJECT
    QML_ELEMENT
public:
    using DrawableProviderFunc = std::function<std::vector<CanvasDrawable*>()>;
    explicit EditorCanvas(QQuickItem* parent = nullptr);
    void paint(QPainter* painter) override;
    void setDrawableProvider(DrawableProviderFunc func);
    [[nodiscard]] std::vector<CanvasDrawable*> getDrawables() const;

    Vec2 getCanvasOffset() const;
    double getCanvasScale() const;
    void setCanvasOffset(const Vec2& offset);
    void setCanvasScale(double scale);

    Q_INVOKABLE void fitToView();

protected:
    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    void wheelEvent(QWheelEvent* event) override;
    bool event(QEvent* event) override;

private:
    DrawableProviderFunc drawableProvider;
    Vec2 canvasOffset;
    double canvasScale;

    bool isPanning;
    QPointF lastMousePos;

    void zoomAt(const QPointF& pos, double zoomFactor);
};

Q_DECLARE_METATYPE(EditorCanvas*)