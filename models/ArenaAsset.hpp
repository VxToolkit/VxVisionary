//
// Created by user on 12/17/25.
//

#pragma once
#include "Asset.hpp"
#include "primitives.hpp"
#include "ArenaElements.hpp"


class ArenaAsset : public Asset {
public:
    [[nodiscard]] [[nodiscard]] AssetType getType() const override;
    void outputData(QDataStream& stream) const override;
    void inputData(QDataStream& stream) override;

protected:
    std::vector<ArenaElement*> elements;
    Transforms2D bounds;
};

