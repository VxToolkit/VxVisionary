//
// Created by user on 12/17/25.
//

#include "Asset.hpp"

QString Asset::getName() {
    return name;
}

void Asset::outputData(QDataStream& stream) {
    stream << name;
}

void Asset::inputData(QDataStream& stream) {
    stream >> name;
}