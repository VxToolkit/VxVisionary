//
// Created by user on 12/16/25.
//

#include "ProjectModel.hpp"
#include <qdir.h>
#include <QDirIterator>


ProjectModel::ProjectModel(QObject* parent) : QAbstractListModel(parent) {
    addProject("test1");
    addProject("test2");
}

ProjectModel::~ProjectModel() = default;

int ProjectModel::rowCount(const QModelIndex& parent) const {
    return static_cast<int>(projects.size());
}

QVariant ProjectModel::data(const QModelIndex& index, int role) const {
    if (!index.isValid() || index.row() < 0 || index.row() >= static_cast<int>(projects.size())) {
        return {};
    }

    const Project& project = projects.at(static_cast<size_t>(index.row()));

    if (role == NameRole) {
        return project.path;
    }

    return {};
}

QHash<int, QByteArray> ProjectModel::roleNames() const {
    QHash<int, QByteArray> roles;

    roles[NameRole] = "name";
    return roles;
}

void ProjectModel::addProject(const QString& name) {
    beginInsertRows(QModelIndex(), projects.size(), projects.size());
    projects.push_back({name});
    endInsertRows();
}
