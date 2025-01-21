#include "headers/SceneManager.h"
#include <iostream>

void SceneManager::addScene(const std::string& name, std::shared_ptr<Scene> scene) {
    scenes[name] = scene;
}

void SceneManager::removeScene(const std::string& name) {
    scenes.erase(name);
}

void SceneManager::switchToScene(const std::string& name) {
    if (scenes.find(name) != scenes.end()) {
        currentScene = scenes[name];
        currentScene->init();
    } else {
        std::cerr << "Scene not found: " << name << std::endl;
    }
}

void SceneManager::updateCurrentScene(float deltaTime) {
    if (currentScene) {
        currentScene->update(deltaTime);
    }
}

void SceneManager::renderCurrentScene() {
    if (currentScene) {
        currentScene->render();
    }
}