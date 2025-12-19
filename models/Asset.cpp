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

Asset::Asset(QString name) : name(std::move(name)) {}