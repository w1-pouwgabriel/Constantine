#include "headers/AssetManager.h"
#include <iostream>

tinygltf::Model AssetManager::loadModel(const std::string& filePath) 
{
    auto it = models.find(filePath);
    if (it != models.end()) {
        return it->second;  // Return the cached model if already loaded
    }

    tinygltf::TinyGLTF loader;
    auto model = tinygltf::Model();
    std::string err, warn;

    bool success = loader.LoadASCIIFromFile(&model, &err, &warn, filePath);
    if (!success) {
        throw std::runtime_error("Failed to load GLTF model: " + err);
    }

    if (!warn.empty()) {
        std::cerr << "TinyGLTF Warning: " << warn << std::endl;
    }

    models[filePath] = model;  // Cache the loaded model

    return model;
}