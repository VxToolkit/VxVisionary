//
// Created by user on 2/14/26.
//

#include <QDir>

class TemplateInfo {
public:
    explicit TemplateInfo();
    explicit TemplateInfo(QDir templatePath);

    int schemaVersion;
    std::string name;
    std::string description;
    std::string version;
    QDir thumbnailPath;

    std::string author;

    std::unordered_map<std::string, QDir> paths;

    std::string errorState;

};
