//
// Created by user on 12/23/25.
//

#include "ArenaPropertyModel.hpp"
#include "ArenaElements.hpp"

ArenaPropertyModel::ArenaPropertyModel(QObject *parent) : QAbstractListModel(parent), targetAsset(nullptr) {}

int ArenaPropertyModel::rowCount(const QModelIndex &parent) const {
    if (!targetAsset) {
        return 0;
    }
    ArenaElementNames names = targetAsset->getPropertyNames();
    return static_cast<int>(names.size());
}

QString ArenaPropertyModel::getPropTypeName(std::any& props) {
    if (props.type() == typeid(int)) {
        return "int";
    } else if (props.type() == typeid(float)) {
        return "float";
    } else if (props.type() == typeid(double)) {
        return "double";
    } else if (props.type() == typeid(QString)) {
        return "string";
    }
    return "unknown";
}

QVariant ArenaPropertyModel::data(const QModelIndex &index, int role) const {
    if (targetAsset == nullptr)
        return {};

    ArenaElementNames names = targetAsset->getPropertyNames();
    ArenaElementProps props = targetAsset->getProperties();

    const QString& currentName = names.at(static_cast<size_t>(index.row()));
    std::any currentProp = props.at(static_cast<size_t>(index.row()));

    if (role == NameRole) {
        return currentName;
    } else if (role == ValueRole) {
        if (currentProp.type() == typeid(int)) {
            return std::any_cast<int>(currentProp);
        } else if (currentProp.type() == typeid(float)) {
            return std::any_cast<float>(currentProp);
        } else if (currentProp.type() == typeid(double)) {
            return std::any_cast<double>(currentProp);
        } else if (currentProp.type() == typeid(QString)) {
            return std::any_cast<QString>(currentProp);
        }
    } else if (role == TypeRole) {
        return getPropTypeName(currentProp);
    }

    return {};
}

QHash<int, QByteArray> ArenaPropertyModel::roleNames() const {
    QHash<int, QByteArray> roles;
    roles[NameRole] = "propName";
    roles[ValueRole] = "propValue";
    roles[TypeRole] = "propType";
    return roles;
}

void ArenaPropertyModel::setTargetElement(ArenaElement* asset) {
    beginResetModel();
    targetAsset = asset;
    endResetModel();
}

bool ArenaPropertyModel::setData(const QModelIndex &index, const QVariant &value, int role) {
    if (targetAsset == nullptr)
        return false;

    if (role == ValueRole) {
        ArenaElementProps props = targetAsset->getProperties();
        const std::any& currentProp = props.at(static_cast<size_t>(index.row()));

        if (currentProp.type() == typeid(int)) {
            targetAsset->setProperty(index.row(), value.toInt());
        } else if (currentProp.type() == typeid(float)) {
            targetAsset->setProperty(index.row(), value.toFloat());
        } else if (currentProp.type() == typeid(double)) {
            targetAsset->setProperty(index.row(), value.toDouble());
        } else if (currentProp.type() == typeid(QString)) {
            targetAsset->setProperty(index.row(), value.toString());
        } else {
            return false;
        }

        emit dataChanged(index, index, {ValueRole});
        emit propertyEdited();
        return true;
    }

    return false;
}