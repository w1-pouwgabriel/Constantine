#include "headers/GraphicsCPU.h"
#include "glfw3.h"
#include "headers/SceneManager.h"
#include "headers/Camera.h" 
#include "headers/Ray.h"
#include "headers/primitive/Circle.h"
#include "headers/primitive/HitResult.h"

#include <iostream>
#include <glm/glm.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/intersect.hpp> // If you want a library function for ray-triangle
#include <string>
#include <chrono>


bool rayTriangleIntersect(
    const glm::vec3& origin,
    const glm::vec3& direction,
    const glm::vec3& v0, 
    const glm::vec3& v1, 
    const glm::vec3& v2, 
    float& t, 
    float& u, 
    float& v)
{
    const float EPSILON = 1e-8f; // Small value to handle floating-point precision
    glm::vec3 edge1 = v1 - v0;
    glm::vec3 edge2 = v2 - v0;

    glm::vec3 h = glm::cross(direction, edge2);
    float a = glm::dot(edge1, h);

    // Check if the ray is parallel to the triangle
    if (a > -EPSILON && a < EPSILON) 
        return false;

    float f = 1.0f / a;
    glm::vec3 s = origin - v0;
    u = f * glm::dot(s, h);

    // Check if the intersection is outside the triangle
    if (u < 0.0f || u > 1.0f) 
        return false;

    glm::vec3 q = glm::cross(s, edge1);
    v = f * glm::dot(direction, q);

    if (v < 0.0f || u + v > 1.0f) 
        return false;

    t = f * glm::dot(edge2, q);

    // Intersection exists if t > EPSILON
    return t > EPSILON;
}

void mouseCallback(GLFWwindow* window, double xpos, double ypos) {
    static float lastX = xpos;
    static float lastY = ypos;

    float deltaX = xpos - lastX;
    float deltaY = lastY - ypos; // Reversed since y-coordinates go from bottom to top

    lastX = xpos;
    lastY = ypos;

    GraphicsCPU* graphics = static_cast<GraphicsCPU*>(glfwGetWindowUserPointer(window));
    if (graphics) {
        graphics->cam.processMouseMovement(deltaX, deltaY);
    }
}


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
    this->cam = Camera(
        glm::vec3(5, 0, 5), // Camera position
        glm::vec3(0, 0, 3), // Circle center
        glm::vec3(0, 1, 0), // Up vector
        90,                 // FOV
        (float)width / height,
        0.0f,
        1.0f
    );
    this->lastTime = std::chrono::high_resolution_clock::now();

    // Make the window's context current
    glfwMakeContextCurrent(window);
    glfwSetCursorPosCallback(window, mouseCallback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetWindowUserPointer(window, this);

    return true;
}

void GraphicsCPU::renderLoop() 
{

    // Main loop
    while (!glfwWindowShouldClose(window)) 
    {
        auto currentTime = std::chrono::high_resolution_clock::now();
        float deltaTime = std::chrono::duration<float>(currentTime - lastTime).count();
        lastTime = currentTime;
        handleInput(deltaTime);

        std::cout << "time: " << deltaTime << std::endl;

        Ray ray;

        Circle circle = Circle(glm::vec3(0.f,0.f,3.f), 0.25f);

        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                float u  = (float)x / width;
                float v = (float)y / height;
                Ray ray = cam.generateRay((float)x / width, (float)y / height);

                auto hit = circle.intersect(ray);

               if (hit) {
                    glm::vec3 color = (hit->normal + 1.0f) * 0.5f;
                    setPixel(x, y, color.r, color.g, color.b);
                    //std::cout << "Hit at pixel (" << x << ", " << y << ") with normal: " << hit->normal.x << ", " << hit->normal.y << ", " << hit->normal.z << std::endl;
                } else {
                    setPixel(x, y, 0.0f, 0.0f, 0.0f);
                }
            }
        }

        glDrawPixels(width, height, GL_RGB, GL_FLOAT, framebuffer.data());
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

void GraphicsCPU::handleInput(float deltaTime) 
{
    // Handle input events
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }

    double mouseX, mouseY;
    glfwGetCursorPos(window, &mouseX, &mouseY);

    if (firstMouse) {
        lastMouseX = mouseX;
        lastMouseY = mouseY;
        firstMouse = false;
    }

    double deltaX = mouseX - lastMouseX;
    double deltaY = lastMouseY - mouseY; // Reversed since y-coordinates go from bottom to top

    lastMouseX = mouseX;
    lastMouseY = mouseY;

    // Apply sensitivity
    float offsetX = static_cast<float>(deltaX) * sensitivity;
    float offsetY = static_cast<float>(deltaY) * sensitivity;

    // Update camera direction
    cam.rotate(glm::radians(offsetY), glm::radians(offsetX));

    // Keyboard Input
    glm::vec3 movement(0.0f);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        movement += cam.direction; // Move forward
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        movement -= cam.direction; // Move backward
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        movement -= cam.right; // Move left
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        movement += cam.right; // Move right
    }

    if (glm::length(movement) > 0.0f) {
        cam.move(glm::normalize(movement) * deltaTime);
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
