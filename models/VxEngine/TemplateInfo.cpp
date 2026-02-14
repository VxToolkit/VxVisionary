//
// Created by user on 2/14/26.
//

#include "TemplateInfo.hpp"
#include "json.hpp"

#define TEMPLATE_JSON_FILENAME "vx_template_schema.json"

using json = nlohmann::json;

TemplateInfo::TemplateInfo() : schemaVersion(0) {

}

TemplateInfo::TemplateInfo(QDir templatePath) : schemaVersion(0) {
    QString jsonFilePath = templatePath.filePath(TEMPLATE_JSON_FILENAME);

    QFileInfo checkFile(jsonFilePath);
    if (!checkFile.exists() || !checkFile.isFile()) {
        errorState = "Template JSON file not found at: " + jsonFilePath.toStdString();
        return;
    }

    QFile jsonFile(jsonFilePath);
    if (!jsonFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        errorState = "Failed to open template JSON file: " + jsonFile.errorString().toStdString();
        return;
    }

    QByteArray fileContents = jsonFile.readAll();
    jsonFile.close();

    json data = json::parse(fileContents.begin(), fileContents.end(), nullptr, false);

    if (data.is_discarded()) {
        errorState = "Invalid JSON format.";
        return;
    }

    schemaVersion = data.value("schemaVersion", 0);
    name = data.value("name", "Unnamed Template");
    description = data.value("description", "");
    version = data.value("version", "0.0.0");
    author = data.value("author", "Unknown");

    for (auto& [key, value] : data["paths"].items()) {
        paths[key] = QDir(QString::fromStdString(value.get<std::string>()));
    }

}