#ifndef GRAPHICS_GL_H
#define GRAPHICS_GL_H

#include "graphics.h"

#include <GLFW/glfw3.h>

class GraphicsGL : public Graphics 
{
public:
    GraphicsGL();

    // Add your methods and members here
        // Initialize the graphics system
    virtual bool initialize(int width, int height, const std::string& title) override;

    // Render a frame
    virtual void renderLoop() override;

    // Set a pixel color at (x, y)
    virtual void setPixel(int x, int y, float r, float g, float b) override {};

    // Save the current frame to an image file
    virtual bool saveFrame(const std::string& filename) override { return true; };

    // Clean up and shut down the graphics system
    virtual void shutdown() override { };

private:
    GLFWwindow* window;
};

#endif // GRAPHICS_GL_H