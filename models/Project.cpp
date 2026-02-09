#include "Project.hpp"
#include <filesystem>
#include <QDir>
#include <QFile>
#include <QDataStream>
#include <QDebug>
#include <algorithm>
#include <qloggingcategory.h>
#include <QStandardPaths>
#include <QtLogging>
#include <QProgressDialog>
#include <QThread>
#include <QEventLoop>
#include <QTimer>
#include <atomic>

#include "ArenaAsset.hpp"
#include "git2.h"

namespace {
    struct GitClonePayload {
        QProgressDialog* dialog;
        std::atomic<bool> canceled{false};
    };

    int fetch_progress(const git_transfer_progress *stats, void *payload)
    {
        auto* data = static_cast<GitClonePayload*>(payload);

        if (data->canceled) return -1;

        if (stats->total_objects > 0) {
            int fetch_percent = (100 * stats->received_objects) / stats->total_objects;
            QString msg = QString("Downloading objects: %1/%2").arg(stats->received_objects).arg(stats->total_objects);

            QMetaObject::invokeMethod(data->dialog, "setLabelText", Qt::QueuedConnection, Q_ARG(QString, msg));
            QMetaObject::invokeMethod(data->dialog, "setValue", Qt::QueuedConnection, Q_ARG(int, fetch_percent));
        }

        return 0;
    }

    void checkout_progress(const char *path, size_t completed_steps, size_t total_steps, void *payload)
    {
        auto* data = static_cast<GitClonePayload*>(payload);
        if (data->canceled) return;

        if (total_steps > 0) {
            int percent = (100 * completed_steps) / total_steps;
            QString msg = QString("Checking out files: %1%").arg(percent);

            QMetaObject::invokeMethod(data->dialog, "setLabelText", Qt::QueuedConnection, Q_ARG(QString, msg));
            QMetaObject::invokeMethod(data->dialog, "setValue", Qt::QueuedConnection, Q_ARG(int, percent));
        }
    }
}

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
                if (QFile::exists(cleanLine) && QFileInfo(cleanLine).isFile()) {
                    projectList.push_back(cleanLine.toStdString());
                } else {
                    qWarning() << "Project file does not exist, skipping:" << cleanLine;
                }
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

    // write source info
    filedata << static_cast<int16_t>(templateSourceType);
    filedata << static_cast<int32_t>(templateSource.size());
    filedata.writeRawData(templateSource.data(), static_cast<int>(templateSource.size()));

    int numOfAssets = static_cast<int>(assets.size());
    filedata << numOfAssets;
    for (const auto& asset : assets) {
        asset->outputData(filedata);
    }
}

void Project::read_from_datastream(QDataStream& filedata) {
    int version;
    filedata >> version;

    if (version == 1) {
        filedata >> name;

        // load source info
        int16_t sourceTypeInt;
        filedata >> sourceTypeInt;
        templateSourceType = static_cast<TemplateSourceType>(sourceTypeInt);

        int sourceLength;
        filedata >> sourceLength;
        std::string source;
        source.resize(sourceLength);
        filedata.readRawData(source.data(), sourceLength);
        templateSource = std::move(source);

        qDebug() << "loaded sf: " + QString::fromStdString(templateSource) + " type: " + QString::number(static_cast<int>(templateSourceType));

        int numOfAssets;
        filedata >> numOfAssets;
        qDebug() << "num assets: "+ std::to_string(numOfAssets);

        qDeleteAll(assets);
        assets.clear();
        assets.reserve(static_cast<size_t>(numOfAssets));

        for (int i = 0; i < numOfAssets; ++i) {
            int type = 0;
            filedata >> type;
            if (type == static_cast<int>(Vxt::AssetType::Arena)) {
                ArenaAsset* asset = new ArenaAsset(&filedata);
                assets.push_back(dynamic_cast<Asset*>(asset));
            }
            else {
                throw std::runtime_error("Unknown Asset type: "+std::to_string(type));
            }
        }

        assets.shrink_to_fit();
    } else {
        throw std::runtime_error("Failed to read project file: Unsupported version");
    }
}

Project::Project(const QString& projectPath) : templateSourceType(TemplateSourceType::Directory), templateSource("") {
    QFileInfo info(projectPath);
    path = QDir(info.absoluteDir());
    if (!path.exists()) {
        throw std::runtime_error("Directory does not exist: " + projectPath.toStdString());
    }

    QFile file(projectPath);
    if (!file.open(QIODevice::ReadOnly)) {
        throw std::runtime_error("Failed to read project file, directory is not readable: " + projectPath.toStdString());
    }

    QDataStream filedata(&file);

    read_from_datastream(filedata);
}

Project::Project(QString projectName, QString projectPath, TemplateSourceType sourceType, const std::string& source) : name(std::move(projectName)), path(std::move(projectPath)), templateSourceType(sourceType), templateSource(source) {
    getTemplatePath(); // make sure the path is ready
}

Project::~Project() {
    qDeleteAll(assets);
}

QString Project::getName() const {
    return name;
}

QDir Project::getPath() const {
    return path;
}

void Project::save() {
    QFile file(path.filePath(name) + ".vxtemplate");
    if (!file.open(QIODevice::WriteOnly)) {
        throw std::runtime_error("Failed to save project file");
    }

    QDataStream filedata(&file);
    writeToFile(filedata);

    m_dirty = false;
}

void Project::addAsset(Asset* asset) {
    assets.push_back(asset);
}

std::vector<Asset*> Project::getAssetsOfType(Vxt::AssetType type) const {
    std::vector<Asset*> filteredAssets;
    for (const auto& asset : assets) {
        if (asset->getType() == type || type == Vxt::AssetType::Null) {
            filteredAssets.push_back(asset);
        }
    }
    return filteredAssets;
}

std::vector<Asset*>& Project::getAssets() {
    return assets;
}

void Project::setTemplateSource(TemplateSourceType source_t, const std::string& source) {
    templateSourceType = source_t;
    templateSource = source;
}

std::string Project::getTemplateUUID() const {
    std::hash<std::string> hasher;

    if (templateSourceType == TemplateSourceType::Directory) {
        return ""; // no uuid needed for local dir
    }
    else if (templateSourceType == TemplateSourceType::Url) {
        size_t hashValue = hasher(templateSource);
        return std::format("{:016x}", hashValue); // as hex string
    }
    else {
        std::unreachable();
    }
}

QDir Project::cache_url_template(const QDir& templatePath, const std::string& url, const std::string& uuid) {
    QDir full_path = templatePath.filePath(QString::fromStdString(uuid));

    if (!full_path.exists()) {
        QProgressDialog progress("Cloning template...", "Cancel", 0, 100);
        progress.setWindowModality(Qt::WindowModal);
        progress.setMinimumDuration(0);

        GitClonePayload payload;
        payload.dialog = &progress;

        QObject::connect(&progress, &QProgressDialog::canceled, [&payload](){
            payload.canceled = true;
        });

        const char* raw_url = url.c_str();
        std::string pathStr = full_path.path().toStdString();
        const char* destination_path = pathStr.c_str();

        git_clone_options clone_opts = GIT_CLONE_OPTIONS_INIT;
        clone_opts.fetch_opts.callbacks.transfer_progress = fetch_progress;
        clone_opts.fetch_opts.callbacks.payload = &payload;
        clone_opts.checkout_opts.progress_cb = checkout_progress;
        clone_opts.checkout_opts.progress_payload = &payload;

        int error = 0;
        git_repository *repo = nullptr;
        std::string errorMessage;

        QThread* thread = QThread::create([&]() {
            error = git_clone(&repo, raw_url, destination_path, &clone_opts);
            if (error != 0) {
                const git_error* e = git_error_last();
                if (e && e->message) {
                    errorMessage = e->message;
                }
            }
        });

        QEventLoop loop;
        QObject::connect(thread, &QThread::finished, &loop, &QEventLoop::quit);

        progress.show();
        thread->start();
        loop.exec();

        thread->deleteLater();
        progress.close();

        if (error != 0) {
            qDebug() << "Git clone failed:" << QString::fromStdString(errorMessage);
            if (payload.canceled) {
                throw std::runtime_error("Template download canceled by user.");
            }
            throw std::runtime_error("Failed to clone template: " + url + " Error: " + errorMessage);
        }

        git_repository_free(repo);
    }

    return full_path;
}

// will create dir for url source types (if none exists)
QDir Project::getTemplatePath() const {
    QDir writableDir = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    QDir templatesDir = writableDir.filePath("template_downloads");

    if (!templatesDir.exists()) {
        bool result = templatesDir.mkpath(".");

        if (!result) {
            qWarning() << "Failed to create templates directory:" << templatesDir.path();
            throw std::runtime_error("Failed to create templates directory: " + templatesDir.path().toStdString());
        }
    }

    if (templateSourceType == TemplateSourceType::Directory) {
        return {QString::fromStdString(templateSource)};
    }
    else if (templateSourceType == TemplateSourceType::Url) {
        const std::string uuid = getTemplateUUID();
        return cache_url_template(templatesDir, templateSource, uuid);
    }
    throw std::runtime_error("Unknown template source type");
}

void Project::ensureTemplateCached() {
    if (templateSourceType == TemplateSourceType::Url) {
        QTimer::singleShot(0, [this]() {
            try {
                getTemplatePath();
            } catch (const std::exception& e) {
                qWarning() << "Failed to cache template:" << e.what();
            }
        });
    }
}

void Project::markDirty() {
    m_dirty = true;
}

bool Project::isDirty() const {
    return m_dirty;
}

