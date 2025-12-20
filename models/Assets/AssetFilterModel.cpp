//
// Created by user on 12/19/25.
//

#include "AssetFilterModel.hpp"

AssetFilterModel::AssetFilterModel(QObject* parent) : QSortFilterProxyModel(parent) {}

bool AssetFilterModel::filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const {
    QModelIndex index = sourceModel()->index(sourceRow, 0, sourceParent);
    QVariant typeData = sourceModel()->data(index, Qt::UserRole + 1);
    if (!typeData.isValid()) {
        return false;
    }
    Vxt::AssetType assetType = static_cast<Vxt::AssetType>(typeData.toInt());
    return assetType == acceptedType || acceptedType == Vxt::AssetType::Null;
}

void AssetFilterModel::setAcceptedType(Vxt::AssetType type) {
    acceptedType = type;
    emit acceptedTypeChanged();
    invalidate();
}

Vxt::AssetType AssetFilterModel::getAcceptedType() const {
    return acceptedType;
}