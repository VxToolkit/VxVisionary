//
// Created by Marco Stulic on 12/18/25.
//

#include "ArenaEditor.hpp"

#include <qqmlcontext.h>
#include <QQuickWindow>
#include <QInputDialog>
#include <QMessageBox>

#include "AppController.h"
#include "models/ArenaAsset.hpp"
#include "models/ArenaElementRegistry.hpp"

EditorType ArenaEditor::type() const {
    return EditorType::Arena;
}

ArenaEditor::ArenaEditor(QObject* parent, QQmlApplicationEngine* engine, QObject* window, AppController *controller) : vxEditor(parent, engine, window, controller) {
    m_engine->rootContext()->setContextProperty("arenaEditor", this);
    m_window->setProperty("visible", true);
    QQuickWindow *quickWindow = qobject_cast<QQuickWindow*>(m_window);
    connect(quickWindow, &QQuickWindow::closing, this, []() {
        AppController::deleteEditorPtr(EditorType::Arena);
    });

    currentElementsModel = new ArenaElementsModel(this);
}

ArenaEditor::~ArenaEditor() {
    m_window->setProperty("visible", false);
}

void ArenaEditor::newArena() const {
    bool ok;
    QString newName = QInputDialog::getText(
        nullptr,
        "Enter the name for the new arena:",
        "Name:",
        QLineEdit::Normal,
        "Untitled Arena",
        &ok
    );

    if (ok && !newName.isEmpty()) {
        ArenaAsset *newArena = new ArenaAsset(newName, {100,100});
        m_controller->currentLoadedProject()->addAsset(dynamic_cast<Asset*>(newArena));
    }
}

void ArenaEditor::openArena() {
    m_controller->askForAsset(this, Vxt::AssetType::Arena);
}

void ArenaEditor::loadArena(ArenaAsset* assetToLoad) {
    qWarning() << "Loading arena asset:" << assetToLoad->getName();
    activeArena = assetToLoad;

    bool alreadyOpen = false;
    for (auto* arena : openArenas) {
        if (arena == assetToLoad) {
            alreadyOpen = true;
            break;
        }
    }

    if (!alreadyOpen) {
        openArenas.push_back(assetToLoad);
    }
    currentElementsModel->setArenaAsset(activeArena);

    emit arenaChanged();
    emit elementsChanged();
    emit tabUpdateEvent();
    currentElementsModel->reset();
}

void ArenaEditor::assetRecieved(Asset* asset) {
    ArenaAsset* arenaAsset = dynamic_cast<ArenaAsset*>(asset);
    if (arenaAsset) {
        loadArena(arenaAsset);
    } else {
        QMessageBox::warning(nullptr, "Error", "Please select a new arena asset.");
    }
}

ArenaAsset* ArenaEditor::getCurrentArena() const {
    return activeArena;
}

QStringList ArenaEditor::getTabs() {
    QStringList tabNames;
    for (const auto& arena : openArenas) {
        tabNames.append(arena->getName());
    }
    return tabNames;
}

void ArenaEditor::setCurrentTab(QString name) {
    for (const auto& arena : openArenas) {
        if (arena->getName() == name) {
            loadArena(arena);
            return;
        }
    }
}

void ArenaEditor::deleteTab(QString name) {
    for (auto it = openArenas.begin(); it != openArenas.end(); ++it) {
        if ((*it)->getName() == name) {
            openArenas.erase(it);
            emit tabUpdateEvent();
            if (activeArena && activeArena->getName() == name) {
                activeArena = openArenas.empty() ? nullptr : openArenas.front();
                emit arenaChanged();
            }
            return;
        }
    }
}

void ArenaEditor::addElementToCurrentArena() {
    if (!activeArena) {
        QMessageBox::warning(nullptr, "Error", "No active arena to add elements to.");
        return;
    }
    QStringList elements = ArenaElementRegistry::getRegisteredElementNames();
    m_controller->askForGenericElement(this, [this](QString picker) {
        ArenaElement* newElement = ArenaElementRegistry::createElement(picker.toStdString());
        if (newElement) {
            newElement->name = picker;
            activeArena->addElement(newElement);
            emit arenaChanged();
        } else {
            QMessageBox::warning(nullptr, "Error", "Failed to create the selected element.");
        }
    }, "Select Element to Add", elements);
    emit elementsChanged();
    currentElementsModel->reset();
}

QStringList ArenaEditor::getCurrentArenaElements() const {
    QStringList elementNames;
    if (activeArena) {
        for (const auto& element : activeArena->getElements()) {
            elementNames.append(element->name);
        }
    }
    return elementNames;
}

ArenaElementsModel* ArenaEditor::currentElementsModelRead() const {
    return currentElementsModel;
}