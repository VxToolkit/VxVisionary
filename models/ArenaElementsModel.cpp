//
// Created by Marco Stulic on 12/23/25.
//

#include "ArenaElementsModel.hpp"

ArenaElementsModel::ArenaElementsModel(QObject* parent) : QAbstractItemModel(parent), m_arenaAsset(nullptr) {}

QModelIndex ArenaElementsModel::index(int row, int column, const QModelIndex& parent) const {
    if (!m_arenaAsset || row < 0 || column < 0 || column >= 1) {
        return QModelIndex();
    }

    if (!parent.isValid()) {
        if (row < static_cast<int>(m_arenaAsset->getElements().size())) {
            return createIndex(row, column, nullptr);
        }
    }

    return QModelIndex();
}

QModelIndex ArenaElementsModel::parent(const QModelIndex& index) const {
    return QModelIndex();
}

int ArenaElementsModel::rowCount(const QModelIndex& parent) const {
    if (!m_arenaAsset) {
        return 0;
    }

    if (!parent.isValid()) {
        return static_cast<int>(m_arenaAsset->getElements().size());
    }

    return 0;
}

int ArenaElementsModel::columnCount(const QModelIndex& parent) const {
    return 1;
}

QVariant ArenaElementsModel::data(const QModelIndex& index, int role) const {
    if (!m_arenaAsset || !index.isValid() || index.row() < 0 || index.row() >= static_cast<int>(m_arenaAsset->getElements().size())) {
        return {};
    }

    ArenaElement* element = m_arenaAsset->getElements().at(static_cast<size_t>(index.row()));

    if (role == Qt::DisplayRole) {
        return element->name;
    }

    return {};
}

void ArenaElementsModel::setArenaAsset(ArenaAsset* arenaAsset) {
    beginResetModel();
    m_arenaAsset = arenaAsset;
    endResetModel();
}

void ArenaElementsModel::reset() {
    beginResetModel();
    endResetModel();
}