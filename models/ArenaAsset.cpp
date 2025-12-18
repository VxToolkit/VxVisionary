//
// Created by user on 12/17/25.
//

#include "ArenaAsset.hpp"

#include "CopyUtils.hpp"

AssetType ArenaAsset::getType() const {
    return AssetType::Arena;
}

void ArenaAsset::outputData(QDataStream& stream) const {
    Asset::outputData(stream);
    int numElements = static_cast<int>(elements.size());
    stream << numElements;
    for (const auto& element : elements) {
        stream << static_cast<int>(element->getType());
        element->outputData(stream);
    }
    CopyUtils::writeTransforms2D(bounds, stream);

}

void ArenaAsset::inputData(QDataStream& stream) {
    Asset::inputData(stream);
    int numElements;
    stream >> numElements;
    elements.clear();
    for (int i = 0; i < numElements; ++i) {
        int type = 0;
        stream >> type;
        if (type == static_cast<int>(ArenaElementType::NoGo)) {
            NoGoElement* element = new NoGoElement();

            element->inputData(stream);
            elements.push_back(dynamic_cast<ArenaElement*>(element));
        } else if (type == static_cast<int>(ArenaElementType::Goal)) {
            GoalElement* element = new GoalElement();

            element->inputData(stream);
            elements.push_back(dynamic_cast<ArenaElement*>(element));
        } else {
            throw std::runtime_error("Unknown Arena Asset type: "+std::to_string(type));
        }
    }
}