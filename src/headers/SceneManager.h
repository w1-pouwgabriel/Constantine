#ifndef SCENEMANAGER_H
#define SCENEMANAGER_H

#include <memory>
#include <unordered_map>
#include <string>

class Scene 
{
public:
    virtual void init() = 0;
    virtual void update(float deltaTime) = 0;
    virtual void render() = 0;
    virtual ~Scene() = default;
};

class Circles : Scene 
{
public:
    void init() override {};
    void update(float deltaTime) override {};
    void render() override {};
};

class SceneManager 
{
public:
    static SceneManager& getInstance() {
        static SceneManager instance;
        return instance;
    }

    void addScene(const std::string& name, std::shared_ptr<Scene> scene);
    void removeScene(const std::string& name);
    void switchToScene(const std::string& name);
    void updateCurrentScene(float deltaTime);
    void renderCurrentScene();

private:
    SceneManager() = default;
    std::unordered_map<std::string, std::shared_ptr<Scene>> scenes;
    std::shared_ptr<Scene> currentScene;
};

#endif // SCENEMANAGER_H