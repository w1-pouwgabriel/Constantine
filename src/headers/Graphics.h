#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <string>
#include <vector>

class Graphics {
public:
    virtual ~Graphics() = default;

    // Initialize the graphics system
    virtual bool initialize(int width, int height, const std::string& title) = 0;

    // Render a frame
    virtual void renderLoop() = 0;

    // Set a pixel color at (x, y)
    virtual void setPixel(int x, int y, float r, float g, float b) = 0;

    // Save the current frame to an image file
    virtual bool saveFrame(const std::string& filename) = 0;

    // Function to handle input events
    virtual void handleInput() = 0;

    // Clean up and shut down the graphics system
    virtual void shutdown() = 0;

protected:
    int width, height;
    std::vector<float> framebuffer;
};

#endif // GRAPHICS_H