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

ArenaEditor::ArenaEditor(QObject* parent, QQmlApplicationEngine* engine, QObject* window, AppController *controller)
    : vxEditor(parent, engine, window, controller), activeArena(nullptr), editorCanvas(nullptr) {
    auto *quickWindow = qobject_cast<QQuickWindow*>(m_window);
    connect(quickWindow, &QQuickWindow::closing, this, []() {
        AppController::deleteEditorPtr(EditorType::Arena);
    });

    currentElementsModel = new ArenaElementsModel(this);
    currentPropertyModel = new ArenaPropertyModel(this);
}

ArenaEditor::~ArenaEditor() = default;

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
    currentElementsModel->reset();
}

void ArenaEditor::canvasReady(EditorCanvas* canvas) {
    qDebug() << "The canvas is ready!";
    editorCanvas = canvas;

    editorCanvas->setDrawableProvider([this]() {
        std::vector<CanvasDrawable*> drawables;
        if (activeArena) {
            for (auto& element : activeArena->getElements()) {
                drawables.push_back(element);
            }
        }
        return drawables;
    });

    connect(this, &ArenaEditor::arenaChanged, editorCanvas, &QQuickItem::update);
    connect(this, &ArenaEditor::elementsChanged, editorCanvas, &QQuickItem::update);
    connect(currentPropertyModel, &QAbstractListModel::dataChanged, editorCanvas, [this]() {
        qDebug() << "Property model data changed";
        if (editorCanvas) editorCanvas->update();
    });

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
    currentPropertyModel->setTargetElement(nullptr);

    emit arenaChanged();
    emit elementsChanged();
    emit arenaPropModelChanged();
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

void ArenaEditor::setCurrentTab(int index) {
    if (index >= 0 && index < static_cast<int>(openArenas.size())) {
        loadArena(openArenas.at(index));
    }
}

void ArenaEditor::deleteTab(int index) {
    if (index >= 0 && index < static_cast<int>(openArenas.size())) {
        ArenaAsset* toRemove = openArenas.at(index);
        openArenas.erase(openArenas.begin() + index);
        emit tabUpdateEvent();

        if (toRemove == activeArena) {
            if (!openArenas.empty()) {
                int newIndex = std::min(index, static_cast<int>(openArenas.size()) - 1);
                loadArena(openArenas.at(newIndex));
            } else {
                activeArena = nullptr;
                currentElementsModel->setArenaAsset(nullptr);
                currentPropertyModel->setTargetElement(nullptr);
                emit arenaChanged();
                emit elementsChanged();
                emit arenaPropModelChanged();
            }
        }
    }
}

void ArenaEditor::addElementToCurrentArena() {
    if (!activeArena) {
        QMessageBox::warning(nullptr, "Error", "No active arena to add elements to.");
        return;
    }
    QStringList elements = ArenaElementRegistry::getRegisteredElementNames();
    m_controller->askForGenericElement(this, [this](const QString& picker) {
        ArenaElement* newElement = ArenaElementRegistry::createElement(picker.toStdString());
        if (newElement) {
            qDebug() << "adding new element " << picker;
            newElement->name = picker;
            activeArena->addElement(newElement);
            emit arenaChanged();
        } else {
            QMessageBox::warning(nullptr, "Error", "Failed to create the selected element.");
        }
        emit elementsChanged();
        currentElementsModel->reset();
    }, "Select Element to Add", elements);
}

void ArenaEditor::selectElementAtIndex(int index) {
    if (!activeArena || index < 0 || index >= static_cast<int>(activeArena->getElements().size())) {
        selectedElementIndex = -1;
        currentPropertyModel->setTargetElement(nullptr);
        return;
    }

    selectedElementIndex = index;
    ArenaElement* element = activeArena->getElements().at(static_cast<size_t>(index));
    currentPropertyModel->setTargetElement(element);
}

void ArenaEditor::removeSelectedElementFromCurrentArena() {
    if (!activeArena || selectedElementIndex < 0 || selectedElementIndex >= static_cast<int>(activeArena->getElements().size())) {
        return;
    }

    auto& elements = activeArena->getElements();
    elements.erase(elements.begin() + selectedElementIndex);

    selectedElementIndex = -1;
    currentPropertyModel->setTargetElement(nullptr);

    emit elementsChanged();
    emit arenaChanged();
    currentElementsModel->reset(); // refresh list
}

QStringList ArenaEditor::getCurrentArenaElements() const {
    QStringList elementNames;
    if (activeArena) {
        for (const auto& element : activeArena->getElements()) {
            elementNames.append(element->name);
        }
    }
    for (const auto& name : elementNames) {
        qDebug() << "Element: " << name;
    }
    return elementNames;
}

ArenaElementsModel* ArenaEditor::currentElementsModelRead() const {
    return currentElementsModel;
}

ArenaPropertyModel* ArenaEditor::currentPropertyModelRead() const {
    return currentPropertyModel;
}

void ArenaEditor::selectElement(int index) {
    if (!activeArena) {
        QMessageBox::warning(nullptr, "Error", "No active arena to select elements from.");
        return;
    }
    if (index < 0 || index >= static_cast<int>(activeArena->getElements().size())) {
        QMessageBox::warning(nullptr, "Error", "The selected element idx is not inbounds.");
        return;
    }
    ArenaElement* selectedElement = activeArena->getElements().at(static_cast<size_t>(index));
    currentPropertyModel->setTargetElement(selectedElement);
    selectedElementIndex = index;
    emit arenaPropModelChanged();
}

ArenaElement* ArenaEditor::getSelectedElement() const {
    return activeArena->getElements().at(static_cast<size_t>(selectedElementIndex));
}