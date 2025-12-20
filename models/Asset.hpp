//
// Created by user on 12/17/25.
//

#pragma once
#include <QString>

namespace Vxt
{
    Q_NAMESPACE
    enum class AssetType : int {
        Null = -1,
        Arena
    };
    Q_ENUM_NS(AssetType)
}

class Asset {
public:
    virtual ~Asset() = default;
    explicit Asset(QString name);

    QString getName();
    virtual void outputData(QDataStream& stream) const;
    virtual void inputData(QDataStream& stream);
    [[nodiscard]] virtual Vxt::AssetType getType() const = 0;

    unsigned int getLoadedInstances() const;
    void incrementLoadedInstances();
    void decrementLoadedInstances();
    void resetLoadedInstances();

protected:
    QString name;
    unsigned int loadedInstances = 0;
};

