//
// Created by user on 12/17/25.
//

#pragma once
#include <QString>

enum class AssetType : int {
    Arena
};

class Asset {
public:
    virtual ~Asset() = default;
    explicit Asset(QString name);

    QString getName();
    virtual void outputData(QDataStream& stream) const;
    virtual void inputData(QDataStream& stream);
    [[nodiscard]] virtual AssetType getType() const = 0;

protected:
    QString name;
};

