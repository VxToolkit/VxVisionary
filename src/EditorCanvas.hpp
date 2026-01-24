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

#include "primitives.hpp"

constexpr double CANVAS_PPI = 100.0;

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

    private:
    DrawableProviderFunc drawableProvider;
    Vec2 canvasOffset;
    double canvasScale;
};

Q_DECLARE_METATYPE(EditorCanvas*)