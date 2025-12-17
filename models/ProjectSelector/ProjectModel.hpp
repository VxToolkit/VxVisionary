//
// Created by user on 12/16/25.
//

#pragma once

#include <QAbstractListModel>

struct ListedProject {
    QString path;
};

class ProjectModel : public QAbstractListModel {
    Q_OBJECT

public:
    explicit ProjectModel(QObject* parent = nullptr);
    ~ProjectModel() override;

    enum ProjectRoles {
        NameRole = Qt::UserRole + 1,
    };

    [[nodiscard]] int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    [[nodiscard]] QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    [[nodiscard]] QHash<int, QByteArray> roleNames() const override;

    void addProject(const QString &name);

private:
    std::vector<ListedProject> projects;
};
