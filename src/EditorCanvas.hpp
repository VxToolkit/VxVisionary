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

class EditorCanvas : public QQuickPaintedItem {
    Q_OBJECT
    QML_ELEMENT
public:
    using DrawableProviderFunc = std::function<std::vector<CanvasDrawable*>()>;
    explicit EditorCanvas(QQuickItem* parent = nullptr);
    void paint(QPainter* painter) override;
    void setDrawableProvider(DrawableProviderFunc func);
    [[nodiscard]] std::vector<CanvasDrawable*> getDrawables() const;

private:
    DrawableProviderFunc drawableProvider;
};

Q_DECLARE_METATYPE(EditorCanvas*)