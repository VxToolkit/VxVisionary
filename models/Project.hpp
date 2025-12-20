#pragma once

#include <QDir>
#include <vector>
#include <string>

#include "Asset.hpp"

class Project {
public:

    explicit Project(const QString& projectPath);
    Project(QString projectName, QString projectPath);
    void writeToFile(QDataStream& filedata) const;
    void save() const;
    void addAsset(Asset* asset);

    std::vector<Asset*>& getAssets();
    std::vector<Asset*> getAssetsOfType(Vxt::AssetType type) const;

    void read_from_datastream(QDataStream& filedata);

    [[nodiscard]] QString getName() const;
    [[nodiscard]] QDir getPath() const;

    // statics
    static void getProjectList(const QDir& configPath);
    static std::vector<std::string> getRecentProjects();
    static void addRecentProject(const std::string& projectPath);
    static void writeRecentProjects(const QDir& configPath);

protected:
    QString name;
    QDir path;
    // statics
    static std::vector<std::string> recent_projects;
    std::vector<Asset*> assets;
};
