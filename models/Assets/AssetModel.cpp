//
// Created by user on 12/19/25.
//

#include "AssetModel.hpp"

AssetModel::AssetModel(QObject* parent, AppController* controller) : QAbstractListModel(parent), m_controller(controller) {}

AssetModel::~AssetModel() = default;

int AssetModel::rowCount(const QModelIndex &parent) const {
    if (parent.isValid()) {
        return 0;
    }
    return static_cast<int>(m_controller->getAssetsOfType(Vxt::AssetType::Null).size());
}

QVariant AssetModel::data(const QModelIndex &index, int role) const {
    if (!index.isValid()) return {};

    const auto& allAssets = m_controller->getAssets();

    if (index.row() >= allAssets.size()) return {};

    Asset* asset = allAssets.at(index.row());

    if (role == NameRole) return asset->getName();
    if (role == TypeRole) return static_cast<int>(asset->getType());

    return {};
}

QHash<int, QByteArray> AssetModel::roleNames() const {
    QHash<int, QByteArray> roles;

    roles[NameRole] = "name";
    roles[TypeRole] = "type";
    return roles;
}