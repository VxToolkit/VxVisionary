//
// Created by Marco Stulic on 12/18/25.
//

#pragma once
#include <qtmetamacros.h>

#include "AppController.h"
#include "vxEditor.hpp"
#include "models/ArenaAsset.hpp"
#include "models/ArenaElementsModel.hpp"

class ArenaEditor : public vxEditor {
    Q_OBJECT
    Q_PROPERTY(ArenaAsset* currentArena READ getCurrentArena NOTIFY arenaChanged)
    Q_PROPERTY(QStringList tabs READ getTabs NOTIFY tabUpdateEvent)
    Q_PROPERTY(QStringList currentArenaElements READ getCurrentArenaElements NOTIFY elementsChanged)
    Q_PROPERTY(ArenaElementsModel* currentElementsModel READ currentElementsModelRead NOTIFY elementsChanged)
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

    QStringList getCurrentArenaElements() const;

    ArenaAsset* getCurrentArena() const;

    QStringList getTabs();
    ArenaElementsModel* currentElementsModelRead() const;

signals:
    void arenaChanged();
    void tabUpdateEvent();
    void elementsChanged();

protected:
    std::vector<ArenaAsset*> openArenas;
    ArenaAsset* activeArena;
    ArenaElementsModel* currentElementsModel;
};
