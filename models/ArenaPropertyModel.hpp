//
// Created by user on 12/23/25.
//

#pragma once
#include <QAbstractListModel>

#include "ArenaAsset.hpp"

class ArenaPropertyModel : public QAbstractListModel {
    Q_OBJECT
public:
    enum Roles {
        NameRole = Qt::UserRole + 1,
        ValueRole,
        TypeRole
    };

    explicit ArenaPropertyModel(QObject *parent = nullptr);
    [[nodiscard]] int rowCount(const QModelIndex &parent) const override;
    [[nodiscard]] QVariant data(const QModelIndex &index, int role) const override;
    [[nodiscard]] QHash<int, QByteArray> roleNames() const override;
    bool setData(const QModelIndex &index, const QVariant &value, int role) override;

    [[nodiscard]] static QString getPropTypeName(std::any& props);

    void setTargetElement(ArenaElement* asset);
private:
    ArenaElement* targetAsset;
};
