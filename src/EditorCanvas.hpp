//
// Created by Marco Stulic on 1/21/26.
//

#pragma once
#include <QQuickPaintedItem>

class EditorCanvas : public QQuickPaintedItem {
    Q_OBJECT
public:
    explicit EditorCanvas(QQuickItem* parent = nullptr);
    void paint(QPainter* painter) override;
};
