#ifndef GRAPHICS_CPU_H
#define GRAPHICS_CPU_H

#include <GLFW/glfw3.h>

#include "Graphics.h"
#include "Camera.h"
#include "TriangleMesh.h"
#include "primitive/Circle.h"
#include "light/PointLight.h"

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

    void addCircle(Circle& circle) { circles.emplace_back(circle); };
    void addMesh(TriangleMesh& mesh) { meshes.emplace_back(mesh); };
    void addLight(const PointLight& light) { lights.emplace_back(light); };

    Camera cam;

  private:
    GLFWwindow* window;
    std::vector<TriangleMesh> meshes;
    std::vector<PointLight> lights;
    std::vector<Circle> circles;

    double lastMouseX, lastMouseY;
    bool captureInput = false;
    bool rightMousePressed = false;
};

#endif // GRAPHICS_CPU_H

