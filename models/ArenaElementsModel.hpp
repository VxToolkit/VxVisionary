//
// Created by Marco Stulic on 12/23/25.
//

#pragma once
#include "ArenaAsset.hpp"
#include "Assets/AssetFilterModel.hpp"

class ArenaElementsModel : public QAbstractItemModel {
    Q_OBJECT
public:
    explicit ArenaElementsModel(QObject* parent = nullptr);
    ~ArenaElementsModel() override = default;

    [[nodiscard]] QModelIndex index(int row, int column, const QModelIndex& parent) const override;
    [[nodiscard]] QModelIndex parent(const QModelIndex& index) const override;
    [[nodiscard]] int rowCount(const QModelIndex& parent) const override;
    [[nodiscard]] int columnCount(const QModelIndex& parent) const override;
    [[nodiscard]] QVariant data(const QModelIndex& index, int role) const override;
    QHash<int, QByteArray> roleNames() const override;
    void setArenaAsset(ArenaAsset* arenaAsset);
    void reset();

private:
    ArenaAsset* m_arenaAsset;
};