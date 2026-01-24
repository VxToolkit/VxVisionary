//
// Created by user on 12/17/25.
//

#pragma once
#include "Asset.hpp"
#include "primitives.hpp"
#include "ArenaElements.hpp"


class ArenaAsset : public Asset {
    Q_OBJECT
public:

    ArenaAsset(QString name, Vec2 bounds, QObject* parent = nullptr);
    ArenaAsset(QDataStream* stream, QObject* parent = nullptr);
    [[nodiscard]] Vxt::AssetType getType() const override;
    void outputData(QDataStream& stream) const override;
    void inputData(QDataStream& stream) override;
    Vec2 const& getBounds() const;
    std::vector<ArenaElement*>& getElements();

    void addElement(ArenaElement* element);

protected:
    std::vector<ArenaElement*> elements;
    Vec2 bounds;
};
