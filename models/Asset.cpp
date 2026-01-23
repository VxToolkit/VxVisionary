//
// Created by user on 12/17/25.
//

#include "Asset.hpp"
#include <QDataStream>

QString Asset::getName() {
    return name;
}

void Asset::outputData(QDataStream& stream) const {
    stream << getType();
    stream << name;
}

void Asset::inputData(QDataStream& stream) {
    stream >> name;
}

Asset::Asset(QString name, QObject* parent) : QObject(parent), name(std::move(name)) {}

unsigned int Asset::getLoadedInstances() const {
    return loadedInstances;
}

void Asset::incrementLoadedInstances() {
    loadedInstances++;
}

void Asset::decrementLoadedInstances() {
    if (loadedInstances > 0) {
        loadedInstances--;
    }
}

void Asset::resetLoadedInstances() {
    loadedInstances = 0;
}