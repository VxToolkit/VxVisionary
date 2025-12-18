//
// Created by user on 12/17/25.
//

#pragma once
#include <QString>

enum class AssetType {
    Arena
};

class Asset {
public:
    virtual ~Asset() = default;

    QString getName();
    virtual void outputData(QDataStream& stream);
    virtual void inputData(QDataStream& stream);
    virtual AssetType getType() = 0;

protected:
    QString name;
};

