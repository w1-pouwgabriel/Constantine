#ifndef GRAPHICS_GL_H
#define GRAPHICS_GL_H

#include "Graphics.h"
#include "Camera.h" 
#include <GLFW/glfw3.h>

class Camera;
class TriangleMesh;

class GraphicsCPU : public Graphics 
{
public:
    // Add your methods and members here
    //  Initialize the graphics system
    virtual bool initialize(int width, int height, const std::string& title) override;

    // Render a frame
    virtual void renderLoop() override;

    // Set a pixel color at (x, y)
    virtual void setPixel(int x, int y, float r, float g, float b) override;

    // Save the current frame to an image file
    virtual bool saveFrame(const std::string &filename) override;

    // Function to handle input events
    virtual void handleInput(float deltaTime) override;

    // Clean up and shut down the graphics system
    virtual void shutdown() override;

    void addMesh(TriangleMesh& mesh);

    Camera cam;

  private:
    GLFWwindow* window;
    std::vector<TriangleMesh> meshes;

    double lastMouseX, lastMouseY;
    bool captureInput = false;
    bool rightMousePressed = false;
    float sensitivity = 1.25f;
};

#endif // GRAPHICS_GL_H

