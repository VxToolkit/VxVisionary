#pragma once

#include <QObject>
#include <QString>
#include <QtQml/qqml.h>
#include <QQmlApplicationEngine>

#include "Editors/vxEditor.hpp"
#include "models/Project.hpp"
#include "models/ProjectSelector/ProjectModel.hpp"

class AppController : public QObject {
    Q_OBJECT
    Q_PROPERTY(ProjectModel* projectModel READ projectModel CONSTANT)
    Q_PROPERTY(Project* currentLoadedProject READ currentLoadedProject CONSTANT)
    Q_PROPERTY(QString projectName READ getProjectName NOTIFY projectNameChanged)

public:
    explicit AppController(QQmlApplicationEngine* engine, QObject* parent = nullptr);
    ProjectModel* projectModel();
    Project *currentLoadedProject();
    QString getProjectName() const;
    Q_INVOKABLE void loadProject(const QString& projectPath);
    Q_INVOKABLE void createNewProject();
    Q_INVOKABLE void openWorkspace(QString name);

    template<typename T>
    T* getEditorOfType(EditorType type) {
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

private:
    QString m_statusMessage;
    ProjectModel m_projectModel;
    QQmlApplicationEngine* m_engine = nullptr;
    Project* m_currentLoadedProject = nullptr;
    static std::vector<vxEditor*> m_openEditors;
    static std::unordered_map<EditorType, QObject*> m_editorWindows;

};
