#pragma once

#include <QObject>
#include <QString>
#include <QtQml/qqml.h>
#include <QQmlApplicationEngine>

#include "Editors/vxEditor.hpp"
#include "models/Project.hpp"
#include "models/Assets/AssetModel.hpp"
#include "models/ProjectSelector/ProjectModel.hpp"

class AssetModel;

class AppController : public QObject {
    Q_OBJECT
    Q_PROPERTY(ProjectModel* projectModel READ projectModel CONSTANT)
    Q_PROPERTY(Project* currentLoadedProject READ currentLoadedProject CONSTANT)
    Q_PROPERTY(QString projectName READ getProjectName NOTIFY projectNameChanged)
    Q_PROPERTY(AssetModel* assetModel READ assetModel WRITE setAssetModel NOTIFY assetModelChanged)

public:
    explicit AppController(QQmlApplicationEngine* engine, QObject* parent = nullptr);
    ProjectModel* projectModel();
    Project *currentLoadedProject();
    QString getProjectName() const;

    void askForAsset(vxEditor* editor, Vxt::AssetType type);
    void askForGenericElement(vxEditor *editor, std::function<void(QString)> callback, QString title, QStringList data);

    void setAssetModel(AssetModel* model);
    AssetModel* assetModel() const;

    Q_INVOKABLE void loadProject(const QString& projectPath);
    Q_INVOKABLE void createNewProject();
    Q_INVOKABLE void openWorkspace(QString name);
    Q_INVOKABLE void saveCurrentProject();
    Q_INVOKABLE void assetReceived(QString name);
    Q_INVOKABLE void genericElementReceived(QString picker);
    Q_INVOKABLE void loadProjectFromFile();

    std::vector<Asset*>& getAssets() const;

    std::vector<Asset*> getAssetsOfType(Vxt::AssetType type) const;


    template<typename T>
    static T* getEditorOfType(EditorType type) {
        for (vxEditor* editor : m_openEditors) {
            if (editor->type() == type) {
                return dynamic_cast<T*>(editor);
            }
        }
        return nullptr;
    }
    QObject* getEditorWindow(EditorType type);

    static void deleteEditorPtr(EditorType type);

signals:
    void projectNameChanged();
    void assetModelChanged();
    void requestOpenAssetDiag(qint32 type);
    void requestGenericElementPicker(QString title, QStringList data);

private:
    QString m_statusMessage;
    ProjectModel m_projectModel;
    QQmlApplicationEngine* m_engine = nullptr;
    Project* m_currentLoadedProject = nullptr;
    AssetModel* m_assetModel = nullptr;
    static std::vector<vxEditor*> m_openEditors;
    static std::unordered_map<EditorType, QObject*> m_editorWindows;
    vxEditor* m_editorAskedForAsset = nullptr;
    std::function<void(QString)> m_genericElementReceivedCallback = nullptr;

};
