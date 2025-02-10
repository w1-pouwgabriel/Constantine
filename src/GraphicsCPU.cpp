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
#include "headers/TriangleMesh.h"
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
        glm::vec3(-5, 5, -5),
        glm::vec3(0, 0, 1),
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
    // Lights and plane can be precomputed once
    Plane plane(glm::vec3(0, -5, 0), glm::vec3(0, 1, 0));
    addLight(PointLight(glm::vec3(3, 5, 6), glm::vec3(0, 0, 1), 1.0f));
    addLight(PointLight(glm::vec3(-3, 5, -5), glm::vec3(1, 0, 0), 1.0f)); // Red light

    while (!glfwWindowShouldClose(window)) 
    {
        // Calculate frame delta time
        auto currentTime = std::chrono::high_resolution_clock::now();
        float deltaTime = std::chrono::duration<float, std::milli>(currentTime - lastTime).count();
        lastTime = currentTime;

        std::cout << deltaTime << " ms" << std::endl;

        // Handle input for movement and camera interaction
        handleInput(deltaTime);
        
        // Clear framebuffer
        std::fill(framebuffer.begin(), framebuffer.end(), 0.0f);
        
        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                // Generate a ray for the current pixel
                float u = static_cast<float>(x) / width;
                float v = static_cast<float>(y) / height;
                Ray ray = cam.generateRay(u, v);


                glm::vec3 finalColor(0.0f); // Default to black
                float closestT = std::numeric_limits<float>::max();

                for (TriangleMesh& mesh : meshes) {
                    for (Triangle& triangle : mesh.getTriangles()) {
                        // Perform ray-triangle intersection
                        auto hit = triangle.intersect(ray, mesh.getTextures());
                        if (hit && hit->t < closestT) {
                            closestT = hit->t;
                            double extra;
                            double extra2;

                            float x = modf(hit->uv.x, &extra);
                            float y = modf(hit->uv.y, &extra2);
                            
                            finalColor = glm::vec3(x,y,0);
                        }
                    }

                    //Do the plane intersection separately
                    auto hit = plane.intersect(ray);
                    if (hit && hit->t < closestT) {
                        closestT = hit->t;

                        finalColor = hit->color * 0.5f;
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
    framebuffer[(y * width + x) * 3 + 0] += r;
    framebuffer[(y * width + x) * 3 + 1] += g;
    framebuffer[(y * width + x) * 3 + 2] += b;
};

void GraphicsCPU::handleInput(float deltaTime) 
{
    rightMousePressed = false;

    // Handle input events
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }

    double mouseX = width / 2, mouseY = height / 2;
    glfwGetCursorPos(window, &mouseX, &mouseY);

    // Toggle input capture when pressing Right Mouse Button
    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS && !rightMousePressed) {
        captureInput = !captureInput;
        rightMousePressed = true;

        mouseX = lastMouseX;
        mouseX = lastMouseY;
    }
    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_RELEASE && rightMousePressed) {
        //Reset mouse position to prevent sudden jumps
        lastMouseX = mouseX;
        lastMouseY = mouseY;

        rightMousePressed = false;
    }

    if (captureInput) {

        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        glfwGetCursorPos(window, &mouseX, &mouseY);
        double deltaX = mouseX - lastMouseX;
        double deltaY = lastMouseY - mouseY; // Reversed since y-coordinates go from bottom to top

        lastMouseX = mouseX;
        lastMouseY = mouseY;

        // Apply sensitivity
        float offsetX = deltaX * sensitivity;
        float offsetY = deltaY * sensitivity;

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
    } 
    else {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    }

    // Save the current frame to a PNG file when the 'P' key is pressed
    // Note: This will save the frame to the 'frames' directory
    if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS) {
        auto now = std::chrono::system_clock::now();
        auto timeT = std::chrono::system_clock::to_time_t(now);
        std::tm tm = *std::localtime(&timeT); // Convert to local time

        std::ostringstream timestamp;
        timestamp << std::put_time(&tm, "%Y-%m-%d_%H-%M-%S");

        std::string filename = "frames/frame_" + timestamp.str() + ".png";

        saveFrame(filename);

        std::cout << "Saved frame to: " << filename << std::endl;
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
