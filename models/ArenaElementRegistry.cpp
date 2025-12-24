//
// Created by Marco Stulic on 12/23/25.
//

#include "ArenaElementRegistry.hpp"

#include <QList>

ArenaElement* ArenaElementRegistry::createElement(std::string name) {
    auto it = registry.find(name);
    if (it != registry.end()) {
        return it->second();
    }
    return nullptr;
}

void ArenaElementRegistry::registerElement(std::string name, CreateElementFunc func) {
    registry[name] = func;
}

ArenaElementRegistrar::ArenaElementRegistrar(std::string name, ArenaElementRegistry::CreateElementFunc func) {
    ArenaElementRegistry::registerElement(name, func);
}

QStringList ArenaElementRegistry::getRegisteredElementNames() {
    QStringList names;
    for (const auto& pair : registry) {
        names.append(QString::fromStdString(pair.first));
    }
    return names;
}