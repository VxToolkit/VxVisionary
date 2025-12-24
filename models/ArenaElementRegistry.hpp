//
// Created by Marco Stulic on 12/23/25.
//

#pragma once
class ArenaElement;
#include <QString>

class ArenaElementRegistry {
public:
    static ArenaElement* createElement(std::string name);
    using CreateElementFunc = ArenaElement* (*)();
    static void registerElement(std::string name, CreateElementFunc func);
    static QStringList getRegisteredElementNames();
protected:
    static inline std::unordered_map<std::string, CreateElementFunc> registry;
};

struct ArenaElementRegistrar {
    ArenaElementRegistrar(std::string name, ArenaElementRegistry::CreateElementFunc func);
};

#define REGISTER_ARENA_ELEMENT(name ,className) \
namespace { \
ArenaElement* create##className() { \
return new className(); \
} \
static ArenaElementRegistrar registrar##className(name, create##className); \
}
