//
// Created by user on 12/19/25.
//

#pragma once
#include <QAbstractListModel>
#include <qtmetamacros.h>

#include "AppController.h"

class AssetModel : public QAbstractListModel {
    Q_OBJECT

    enum AssetModelRoles {
        NameRole = Qt::DisplayRole,

        TypeRole = Qt::UserRole + 1
    };
public:
    AssetModel(QObject* parent, AppController* controller);
    ~AssetModel() override;
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QHash<int, QByteArray> roleNames() const override;
protected:
    AppController *m_controller = nullptr;
};
