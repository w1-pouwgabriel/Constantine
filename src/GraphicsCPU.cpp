#include "headers/GraphicsCPU.h"
#include "headers/Camera.h" 
#include "headers/Ray.h"
#include "headers/primitive/Circle.h"
#include "headers/primitive/HitResult.h"

#include <iostream>
#include <glm/glm.hpp>

GraphicsCPU::GraphicsCPU() 
{
    // Initialization code
}

// Initialize the graphics system
bool GraphicsCPU::initialize(int width, int height, const std::string& title)
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

void GraphicsCPU::renderLoop() 
{
    Camera cam = Camera(
        glm::vec3(0,0,1),
        glm::vec3(0,0,-1), 
        glm::vec3(0,1,0), 
        90, 
        (float)width / height, 
        0.5f, 
        1.0f
    );

    // Main loop
    while (!glfwWindowShouldClose(window)) 
    {
        handleInput();

        Ray ray = Ray(glm::vec3(0,0,0), glm::vec3(0,0,-1));

        // Simulate ray tracing
        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {

                // Check intersection with a circle
                Circle circle = Circle(0.0f, 0.0f, 0.25f);

                Ray ray = cam.generateRay((float)x / width, (float)y / height);
                auto hit = circle.intersectCircle(ray);

               if (hit) {
                    glm::vec3 color = (hit->normal + 1.0f) * 0.5f;
                    setPixel(x, y, color.r, color.g, color.b);
                    //std::cout << "Hit at pixel (" << x << ", " << y << ") with normal: " << hit->normal.x << ", " << hit->normal.y << ", " << hit->normal.z << std::endl;
                } else {
                    setPixel(x, y, 0.0f, 0.0f, 0.0f);
                }
            }
        }

        // Manually update the window with the framebuffer content
        glDrawPixels(width, height, GL_RGB, GL_FLOAT, framebuffer.data());
        
        // Poll for and process events
        glfwPollEvents();
        glfwSwapBuffers(window);
    }
};

void GraphicsCPU::setPixel(int x, int y, float r, float g, float b) 
{ 
    // Set the pixel color
    framebuffer[(y * width + x) * 3 + 0] = r;
    framebuffer[(y * width + x) * 3 + 1] = g;
    framebuffer[(y * width + x) * 3 + 2] = b;
};

void GraphicsCPU::handleInput() 
{
    // Handle input events
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }
    
};

bool GraphicsCPU::saveFrame(const std::string &filename) 
{
    // Save the current frame to an image file

    return true; 
};

void GraphicsCPU::shutdown() 
{ 
    // Terminate GLFW
    glfwTerminate();
};
