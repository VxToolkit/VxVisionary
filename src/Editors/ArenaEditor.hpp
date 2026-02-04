//
// Created by Marco Stulic on 12/18/25.
//

#pragma once
#include <qtmetamacros.h>

#include "AppController.h"
#include "vxEditor.hpp"
#include "models/ArenaAsset.hpp"
#include "models/ArenaElementsModel.hpp"
#include "models/ArenaPropertyModel.hpp"
#include "EditorCanvas.hpp"
#include "models/Drawables/ArenaDrawable.hpp"

class ArenaEditor : public vxEditor {
    Q_OBJECT
    Q_PROPERTY(ArenaAsset* currentArena READ getCurrentArena NOTIFY arenaChanged)
    Q_PROPERTY(QStringList tabs READ getTabs NOTIFY tabUpdateEvent)
    Q_PROPERTY(QStringList currentArenaElements READ getCurrentArenaElements NOTIFY elementsChanged)
    Q_PROPERTY(ArenaElementsModel* currentElementsModel READ currentElementsModelRead NOTIFY elementsChanged)
    Q_PROPERTY(ArenaPropertyModel* currentPropertyModel READ currentPropertyModelRead NOTIFY arenaPropModelChanged)
    Q_PROPERTY(int selectedElementIndex READ getSelectedElementIndex NOTIFY selectionChanged)
public:
    virtual ~ArenaEditor();
    ArenaEditor(QObject* parent, QQmlApplicationEngine* engine, QObject* window, AppController* controller);

    Q_INVOKABLE void newArena() const;
    Q_INVOKABLE void openArena();
    EditorType type() const override;
    void assetRecieved(Asset* asset) override;
    void loadArena(ArenaAsset* assetToLoad);
    Q_INVOKABLE void setCurrentTab(int index);
    Q_INVOKABLE void deleteTab(int index);
    Q_INVOKABLE void addElementToCurrentArena();
    Q_INVOKABLE void selectElementAtIndex(int index);
    Q_INVOKABLE void removeSelectedElementFromCurrentArena();
    Q_INVOKABLE void canvasReady(EditorCanvas* canvas);

    QStringList getCurrentArenaElements() const;

    ArenaAsset* getCurrentArena() const;

    QStringList getTabs();
    ArenaElementsModel* currentElementsModelRead() const;
    ArenaPropertyModel* currentPropertyModelRead() const;
    void selectElement(int index);
    ArenaElement* getSelectedElement() const;
    int getSelectedElementIndex() const;
    void provideWindow(QObject* window) override;

signals:
    void arenaChanged();
    void arenaPropModelChanged();
    void tabUpdateEvent();
    void elementsChanged();
    void selectionChanged();

protected:
    std::vector<ArenaAsset*> openArenas;
    ArenaAsset* activeArena;
    ArenaElementsModel* currentElementsModel;
    ArenaPropertyModel* currentPropertyModel;
    int selectedElementIndex = -1;
    EditorCanvas* editorCanvas;
    ArenaDrawable *arenaDrawable = nullptr;

};
