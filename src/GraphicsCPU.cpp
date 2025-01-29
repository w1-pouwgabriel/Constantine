#include "glfw3.h"
#include <glm/glm.hpp>
#define GLM_ENABLE_EXPERIMENTAL

#include <iomanip>
#include <sstream>
#include <stb_image.h>
#include <stb_image_write.h>
#include <iostream>
#include <glm/gtx/intersect.hpp> // If you want a library function for ray-triangle
#include <string>
#include <chrono>

#include "headers/GraphicsCPU.h"
#include "headers/Camera.h" 
#include "headers/primitive/Circle.h"
#include "headers/primitive/Plane.h"

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
        glm::vec3(0, 0, 10),
        glm::vec3(0, 0, 3),
        glm::vec3(0, 1, 0),
        90,
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
    while (!glfwWindowShouldClose(window)) 
    {
        // Calculate frame delta time
        auto currentTime = std::chrono::high_resolution_clock::now();
        float deltaTime = std::chrono::duration<float>(currentTime - lastTime).count();
        lastTime = currentTime;

        std::cout << "sec: " << deltaTime << std::endl;

        // Handle input for movement and camera interaction
        handleInput(deltaTime);

        // Clear framebuffer
        std::fill(framebuffer.begin(), framebuffer.end(), 0.0f);

        Plane plane(glm::vec3(0.0f, -25.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));        
        // Create the frustum
        //Frustum frustum;
        //frustum.extractPlanesFromMatrix(cam.getViewMatrix() * cam.getProjectionMatrix());

        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                // Generate a ray for the current pixel
                float u = static_cast<float>(x) / width;
                float v = static_cast<float>(y) / height;
                Ray ray = cam.generateRay(u, v);

                float closestT = std::numeric_limits<float>::max();
                glm::vec3 finalColor(0.0f); // Default to black

                 // Check for plane intersection
                auto hit = plane.intersect(ray);
                if (hit) {
                    finalColor = glm::vec3(.3f, 0.3f, 0.3f);
                }

                // Iterate through all meshes
                for (TriangleMesh& mesh : meshes) 
                {
                    //if (frustum.isAABBInFrustum(mesh.computeAABB())) continue;

                    for (Triangle& triangle : mesh.getTriangles()) 
                    {
                        // Perform ray-triangle intersection
                        auto hit = triangle.intersectFast(ray);
                        if (hit && hit->t < closestT) {
                            closestT = hit->t;
                            finalColor = (hit->normal + 1.0f) * 0.5f; // Convert normal to RGB
                        }
                    }
                }

                // Set the pixel color in the framebuffer
                setPixel(x, y, finalColor.r, finalColor.g, finalColor.b);
            }
        }

        // Draw the framebuffer
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
    static bool rightMousePressed = false; // Track mouse state

    // Handle input events
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }

    double mouseX, mouseY;
    glfwGetCursorPos(window, &mouseX, &mouseY);

    // Toggle input capture when pressing Right Mouse Button
    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS && !rightMousePressed) {
        captureMouse = !captureMouse;
        rightMousePressed = true;

        // Reset mouse position to prevent sudden jumps
        mouseX = lastMouseX;
        mouseX = lastMouseY;
    }
    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_RELEASE && rightMousePressed) {
        // Reset mouse position to prevent sudden jumps
        lastMouseX = mouseX;
        lastMouseY = mouseY;

        rightMousePressed = false;
    }

    // Capture or release the mouse based on `captureMouse`
    if (captureMouse) {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        glfwGetCursorPos(window, &mouseX, &mouseY);
        double deltaX = mouseX - lastMouseX;
        double deltaY = lastMouseY - mouseY; // Reversed since y-coordinates go from bottom to top

        lastMouseX = mouseX;
        lastMouseY = mouseY;

        // Apply sensitivity
        float offsetX = static_cast<float>(deltaX) * sensitivity;
        float offsetY = static_cast<float>(deltaY) * sensitivity;

        // Update camera direction
        cam.rotate(glm::radians(offsetY), glm::radians(offsetX));
    } else {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    }

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
