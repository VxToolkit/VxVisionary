//
// Created by user on 12/16/25.
//

#include "ProjectModel.hpp"
#include <qdir.h>
#include <QDirIterator>


ProjectModel::ProjectModel(QObject* parent) : QAbstractListModel(parent) {

}

ProjectModel::~ProjectModel() = default;

int ProjectModel::rowCount(const QModelIndex& parent) const {
    return static_cast<int>(projects.size());
}

QVariant ProjectModel::data(const QModelIndex& index, int role) const {
    if (!index.isValid() || index.row() < 0 || index.row() >= static_cast<int>(projects.size())) {
        return {};
    }

    const ListedProject& project = projects.at(static_cast<size_t>(index.row()));

    if (role == NameRole) {
        QFileInfo info(project.path);
        return info.baseName();
    }
    else if (role == PathRole) {
        return project.path;
    }

    return {};
}

QHash<int, QByteArray> ProjectModel::roleNames() const {
    QHash<int, QByteArray> roles;

    roles[NameRole] = "name";
    roles[PathRole] = "path";
    return roles;
}

void ProjectModel::addProject(const QString& name) {
    beginInsertRows(QModelIndex(), projects.size(), projects.size());
    projects.push_back({name});
    endInsertRows();
}
