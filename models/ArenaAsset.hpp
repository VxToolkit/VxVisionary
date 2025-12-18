//
// Created by user on 12/17/25.
//

#pragma once
#include "Asset.hpp"

class ArenaElement {

};


class ArenaAsset : public Asset {
public:
    virtual AssetType getType() override;
    virtual void outputData(QDataStream& stream) override;
    virtual void inputData(QDataStream& stream) override;

protected:
    std::vector<ArenaElement> elements;

};
