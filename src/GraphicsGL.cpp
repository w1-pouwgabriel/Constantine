#include "headers/GraphicsGL.h"

#include <iostream>

GraphicsGL::GraphicsGL() {
    // Initialization code
}

// Initialize the graphics system
bool GraphicsGL::initialize(int width, int height, const std::string& title)
{
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return false;
    }

    // Create a windowed mode window and its OpenGL context
    window = glfwCreateWindow(width, height, title.c_str(), NULL, NULL);
    if (!window) {
        glfwTerminate();
        std::cerr << "Failed to open GLFW window" << std::endl;
        return false;
    }
    this->width = width;
    this->height = height;
    this->framebuffer = std::vector<float>(width * height * 3, 0.0f);

    // Make the window's context current
    glfwMakeContextCurrent(window);

    return true;
}

void GraphicsGL::renderLoop() 
{
    // Main loop
    while (!glfwWindowShouldClose(window)) 
    {
        handleInput();

        // Simulate ray tracing
        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                // Calculate color based on position, simple color gradient for demo
                float r = (float)x / width;
                float g = (float)y / height;
                float b = 0.5f;  // Static blue value

                setPixel(x, y, r, g, b);
            }
        }

        // Manually update the window with the framebuffer content
        glDrawPixels(width, height, GL_RGB, GL_FLOAT, framebuffer.data());
        
        // Poll for and process events
        glfwPollEvents();
        glfwSwapBuffers(window);
    }
};

void GraphicsGL::setPixel(int x, int y, float r, float g, float b) 
{ 
    // Set the pixel color
    framebuffer[(y * width + x) * 3 + 0] = r;
    framebuffer[(y * width + x) * 3 + 1] = g;
    framebuffer[(y * width + x) * 3 + 2] = b;
};

void GraphicsGL::handleInput() 
{
    // Handle input events
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }
    
};

bool GraphicsGL::saveFrame(const std::string &filename) 
{ 
    return true; 
};

void GraphicsGL::shutdown() 
{ 
    // Terminate GLFW
    glfwTerminate();
};
