#ifndef SCENEMANAGER_H
#define SCENEMANAGER_H

// #include <memory>
// #include <unordered_map>
// #include <string>
#include <cstdlib>
#include <vector>

#include "primitive/Circle.h"
#include "primitive/TriangleMesh.h"
#include "tiny_gltf.h"

struct Scene 
{
    void init(){};
    void update(float deltaTime){};
    std::vector<Circle> models;
};


// class SceneManager 
// {
// public:
//     static SceneManager& getInstance() {
//         static SceneManager instance;
//         return instance;
//     }

//     void addScene(const std::string& name, std::shared_ptr<Scene> scene);
//     void removeScene(const std::string& name);
//     std::shared_ptr<Scene> getScene(const std::string& name) { return scenes[name]; };
//     void switchToScene(const std::string& name);
//     void updateCurrentScene(float deltaTime);
//     void renderCurrentScene();

// private:
//     SceneManager() = default;
//     std::unordered_map<std::string, std::shared_ptr<Scene>> scenes;
//     std::shared_ptr<Scene> currentScene;
// };

#endif // SCENEMANAGER_H