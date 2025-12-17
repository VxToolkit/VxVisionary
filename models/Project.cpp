#include "Project.hpp"
#include <filesystem>
#include <QDir>
#include <QFile>

std::vector<std::string> Project::recent_projects;
#define PROJECT_FORMAT_VERSION 1

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
    if (std::ranges::find(recent_projects, projectPath) != recent_projects.end()) {
        return;
    }
    recent_projects.push_back(projectPath);
}

void Project::writeToFile(QDataStream& filedata) const {
    filedata << PROJECT_FORMAT_VERSION;
    filedata << name;
}

void Project::read_from_datastream(QDataStream& filedata) {
    int version;
    filedata >> version;

    if (version == 1) {
        filedata >> name;
    } else {
        throw std::runtime_error("Failed to read project file: Unsupported version");
    }
}

Project::Project(const QString& projectPath) {
    QFileInfo info(projectPath);
    path = QDir(info.absoluteDir());
    if (!path.exists()) {
        throw std::runtime_error("Directory does not exist: " + projectPath.toStdString());
    }

    QFile file(projectPath);
    if (!file.open(QIODevice::ReadOnly)) {
        throw std::runtime_error("Failed to read project file");
    }

    QDataStream filedata(&file);

    read_from_datastream(filedata);
}

Project::Project(QString projectName, QString projectPath) : name(projectName), path(projectPath) {}

QString Project::getName() const {
    return name;
}

QDir Project::getPath() const {
    return path;
}

void Project::save() const {
    QFile file(path.filePath(name) + ".vxtemplate");
    if (!file.open(QIODevice::WriteOnly)) {
        throw std::runtime_error("Failed to save project file");
    }

    QDataStream filedata(&file);
    writeToFile(filedata);
}