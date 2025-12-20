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
}

void ArenaEditor::assetRecieved(Asset* asset) {
    ArenaAsset* arenaAsset = dynamic_cast<ArenaAsset*>(asset);
    if (arenaAsset) {
        loadArena(arenaAsset);
    } else {
        QMessageBox::warning(nullptr, "Error", "Please select a new arena asset.");
    }
}