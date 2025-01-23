#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <chrono>
#include <string>
#include <vector>

struct Scene;
class Graphics {
public:
    virtual ~Graphics() = default;

    // Initialize the graphics system
    virtual bool initialize(int width, int height, const std::string& title) = 0;

    // Render a frame
    virtual void renderLoop() = 0;

    // Set a pixel color at (x, y)
    virtual void setPixel(int x, int y, float r, float g, float b) = 0;

    // Change the current scene
    // This will store all the data of the models and lights in the scene
    virtual void setCurrentScene(Scene* newScene) = 0;

    // Save the current frame to an image file
    virtual bool saveFrame(const std::string& filename) = 0;

    // Function to handle input events
    virtual void handleInput(float deltaTime) = 0;

    // Clean up and shut down the graphics system
    virtual void shutdown() = 0;

protected:
    int width, height;
    std::vector<float> framebuffer;
    std::chrono::high_resolution_clock::time_point lastTime;
    Scene* currentScene;
};

#endif // GRAPHICS_H