#pragma once

#include <QDir>
#include <vector>
#include <string>

class Project {
public:
    static void getProjectList(const QDir& configPath);
    static std::vector<std::string> getRecentProjects();
    static void addRecentProject(const std::string& projectPath);
    static void writeRecentProjects(const QDir& configPath);

protected:
    static std::vector<std::string> recent_projects;
};