//
// Created by Marco Stulic on 12/18/25.
//

#pragma once
#include <qtmetamacros.h>

#include "AppController.h"
#include "vxEditor.hpp"
#include "models/ArenaAsset.hpp"

class ArenaEditor : public vxEditor {
    Q_OBJECT
    Q_PROPERTY(ArenaAsset* currentArena READ getCurrentArena NOTIFY arenaChanged)
    Q_PROPERTY(QStringList tabs READ getTabs NOTIFY tabUpdateEvent)
public:
    virtual ~ArenaEditor();
    ArenaEditor(QObject* parent, QQmlApplicationEngine* engine, QObject* window, AppController* controller);

    Q_INVOKABLE void newArena() const;
    Q_INVOKABLE void openArena();
    EditorType type() const override;
    void assetRecieved(Asset* asset) override;
    void loadArena(ArenaAsset* assetToLoad);
    Q_INVOKABLE void setCurrentTab(QString name);
    Q_INVOKABLE void deleteTab(QString name);
    Q_INVOKABLE void addElementToCurrentArena();

    ArenaAsset* getCurrentArena() const;

    QStringList getTabs();

signals:
    void arenaChanged();
    void tabUpdateEvent();

protected:
    std::vector<ArenaAsset*> openArenas;
    ArenaAsset* activeArena;
};
