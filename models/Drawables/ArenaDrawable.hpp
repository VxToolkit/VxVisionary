//
// Created by Marco Stulic on 1/23/26.
//

#pragma once
#include "CanvasDrawable.hpp"
class ArenaAsset;


class ArenaDrawable : public CanvasDrawable {
public:
    ArenaDrawable() = default;
    ~ArenaDrawable() override = default;
    void setLinkedAsset(ArenaAsset* asset);

    void draw(QPainter* painter) override;
private:
    ArenaAsset* linkedAsset = nullptr;
};
