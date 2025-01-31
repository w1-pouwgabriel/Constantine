#ifndef ASSETMANAGER_H
#define ASSETMANAGER_H

#include <string>
#include <unordered_map>
#include "tiny_gltf.h"

class AssetManager {
public:
    static AssetManager& getInstance() {
        static AssetManager instance;
        return instance;
    }

    //Some polymorphism here would be nice 
    //  - Create some kind of Model class that all model types inherit from
    tinygltf::Model loadModel(const std::string& filePath);

private:
    AssetManager() {}
    ~AssetManager() {}

    //There is only 1 instance of AssetManager allowed
    AssetManager(const AssetManager&) = delete;
    AssetManager& operator=(const AssetManager&) = delete;

    std::unordered_map<std::string, tinygltf::Model> models;
};

#endif // ASSETMANAGER_H