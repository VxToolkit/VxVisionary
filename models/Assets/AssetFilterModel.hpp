//
// Created by user on 12/19/25.
//

#pragma once
#include <qtmetamacros.h>
#include <QSortFilterProxyModel>
#include "models/Asset.hpp"

class AssetFilterModel : public QSortFilterProxyModel {
    Q_OBJECT
    Q_PROPERTY(Vxt::AssetType acceptedType READ getAcceptedType WRITE setAcceptedType NOTIFY acceptedTypeChanged)

public:
    explicit AssetFilterModel(QObject* parent = nullptr);
    bool filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const override;
    void setAcceptedType(Vxt::AssetType type);
    Vxt::AssetType getAcceptedType() const;

signals:
    void acceptedTypeChanged();
private:
    Vxt::AssetType acceptedType = Vxt::AssetType::Null;
};
