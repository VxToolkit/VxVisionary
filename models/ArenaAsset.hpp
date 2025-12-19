//
// Created by user on 12/17/25.
//

#pragma once
#include "Asset.hpp"
#include "primitives.hpp"
#include "ArenaElements.hpp"


class ArenaAsset : public Asset {
public:

    ArenaAsset(QString name,Vec2 bounds);
    ArenaAsset(QDataStream* stream);
    [[nodiscard]] [[nodiscard]] AssetType getType() const override;
    void outputData(QDataStream& stream) const override;
    void inputData(QDataStream& stream) override;

    void addElement(ArenaElement* element);

protected:
    std::vector<ArenaElement*> elements;
    Vec2 bounds;
};

