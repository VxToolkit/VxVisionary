#include "Project.hpp"
#include <filesystem>
#include <QDir>
#include <QFile>

std::vector<std::string> Project::recent_projects;

void Project::getProjectList(const QDir& configPath) {
    QFile configFile(configPath.filePath("projects.lst"));
    std::vector<std::string> projectList;

    if (configFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        while (!configFile.atEnd()) {
            QByteArray line = configFile.readLine();
            QString cleanLine = QString::fromUtf8(line).trimmed();

            if (!cleanLine.isEmpty()) {
                projectList.push_back(cleanLine.toStdString());
            }
        }
        configFile.close();
    }
    recent_projects = std::move(projectList);
}

void Project::writeRecentProjects(const QDir& configPath) {
    QFile configFile(configPath.filePath("projects.lst"));

    if (configFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
        for (const auto& projectPath : recent_projects) {
            QString line = QString::fromStdString(projectPath) + "\n";
            configFile.write(line.toUtf8());
        }
        configFile.close();
    }
}

std::vector<std::string> Project::getRecentProjects() {
    return recent_projects;
}

void Project::addRecentProject(const std::string& projectPath) {
    recent_projects.push_back(projectPath);
}
