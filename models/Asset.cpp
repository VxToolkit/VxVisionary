//
// Created by user on 12/17/25.
//

#include "Asset.hpp"
#include <QDataStream>

QString Asset::getName() {
    return name;
}

void Asset::outputData(QDataStream& stream) const {
    stream << name;
    stream << getType();
}

void Asset::inputData(QDataStream& stream) {
    stream >> name;
}