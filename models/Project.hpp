#pragma once

#include <QDir>
#include <vector>
#include <string>

#include "Asset.hpp"
class TemplateInfo;

class Project {
public:
    // source
    enum class TemplateSourceType : int16_t {
        Url,
        Directory
    };

    explicit Project(const QString& projectPath);
    Project(QString projectName, QString projectPath, TemplateSourceType sourceType, const std::string& source);
    ~Project();
    void writeToFile(QDataStream& filedata) const;
    void save();
    void addAsset(Asset* asset);

    std::vector<Asset*>& getAssets();
    std::vector<Asset*> getAssetsOfType(Vxt::AssetType type) const;

    void markDirty();
    bool isDirty() const;

    void read_from_datastream(QDataStream& filedata);

    [[nodiscard]] QString getName() const;
    [[nodiscard]] QDir getPath() const;

    // statics
    static void getProjectList(const QDir& configPath);
    static std::vector<std::string> getRecentProjects();
    static void addRecentProject(const std::string& projectPath);
    static void writeRecentProjects(const QDir& configPath);
    static QDir cache_url_template(const QDir &templatePath, const std::string& url, const std::string &uuid);



    void setTemplateSource(TemplateSourceType source_t,const std::string& source);
    std::string getTemplateUUID() const;

    QDir getTemplatePath() const;
    void ensureTemplateCached();
    void updateTemplate();

    void loadTemplateInfo();

    bool hasTemplateInfo() const;

protected:
    QString name;
    QDir path;
    bool m_dirty = false;
    // statics
    static std::vector<std::string> recent_projects;
    std::vector<Asset*> assets;

    std::string templateSource;
    TemplateSourceType templateSourceType;
    TemplateInfo* templateInfo = nullptr;
};
