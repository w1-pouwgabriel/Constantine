#include "headers/GraphicsCPU.h"
#include "headers/SceneManager.h"
#include "headers/Camera.h" 
#include "headers/primitive/Circle.h"
#include "headers/primitive/HitResult.h"

#include <iomanip>
#include <sstream>
#include <stb_image.h>
#include <stb_image_write.h>
#include <iostream>
#include <glm/glm.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/intersect.hpp> // If you want a library function for ray-triangle
#include <string>
#include <chrono>

void mouseCallback(GLFWwindow* window, double xpos, double ypos) 
{
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
        glm::vec3(3, 0, 5), // Camera position
        glm::vec3(0, 0, 3), // Circle center
        glm::vec3(0, 1, 0), // Up vector
        90,                 // FOV
        (float)width / height,
        0.0f,
        1.0f
    );
    this->lastTime = std::chrono::high_resolution_clock::now();
    for(int i = 0; i < 5; i++)
    {
        circles.push_back(Circle(glm::vec3(rand() % 5, rand() % 5, rand() % 5), .125f));
    }

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

        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                float u = (float)x / width;
                float v = (float)y / height;
                Ray ray = cam.generateRay(u, v);

                float closestT = std::numeric_limits<float>::max();
                glm::vec3 finalColor(0.0f); // Default to black

                // for (auto& circle : circles) {
                //     auto hit = circle.intersect(ray);
                //     if (hit && hit->t < closestT) {
                //         closestT = hit->t;
                //         finalColor = (hit->normal + 1.0f) * 0.5f;
                //     }
                // }

                for (auto& mesh : meshes) {
                    auto hit = mesh.intersect(ray);
                    if (hit && hit->t < closestT) {
                        closestT = hit->t;
                        finalColor = (hit->normal + 1.0f) * 0.5f;
                    }
                }

                // Set the pixel color to the closest hit's color or remain black
                setPixel(x, y, finalColor.r, finalColor.g, finalColor.b);
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
    if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS) {
        // Get the current time
        auto now = std::chrono::system_clock::now();
        auto timeT = std::chrono::system_clock::to_time_t(now);
        std::tm tm = *std::localtime(&timeT); // Convert to local time

        // Format the time as a string for the filename (e.g., YYYY-MM-DD_HH-MM-SS)
        std::ostringstream timestamp;
        timestamp << std::put_time(&tm, "%Y-%m-%d_%H-%M-%S");

        // Create the filename with the timestamp
        std::string filename = "frames/frame_" + timestamp.str() + ".png";

        // Save the frame with the generated filename
        saveFrame(filename);

        std::cout << "Saved frame to: " << filename << std::endl;
    }

    if (glm::length(movement) > 0.0f) {
        cam.move(glm::normalize(movement) * deltaTime);
    }
};

void GraphicsCPU::addMesh(TriangleMesh& mesh) 
{
    meshes.push_back(mesh);
}

bool GraphicsCPU::saveFrame(const std::string &filename) 
{
    // Convert the framebuffer (float) to an 8-bit buffer for stb_image_write
    std::vector<unsigned char> outputBuffer(width * height * 3);
    for (int i = 0; i < width * height * 3; ++i) {
        // Clamp and scale the float values (0.0 to 1.0) to 0-255 range
        outputBuffer[i] = static_cast<unsigned char>(glm::clamp(framebuffer[i], 0.0f, 1.0f) * 255.0f);
    }

    // Write the buffer to a PNG file
    if (stbi_write_png(filename.c_str(), width, height, 3, outputBuffer.data(), width * 3)) {
        std::cout << "Frame saved successfully to: " << filename << std::endl;
        return true;
    } else {
        std::cerr << "Failed to save frame to: " << filename << std::endl;
        return false;
    }
};

void GraphicsCPU::shutdown() 
{ 
    // Terminate GLFW
    glfwTerminate();
};
