#ifndef TEXTURE_H
#define TEXTURE_H

#include <vector>
#include <cstdint>
#include "glm/glm.hpp"

class Texture 
{
public:

    Texture(int w, int h, int comp, std::vector<unsigned char> imgData)
        : width(w), height(h), components(comp), data(imgData) {}

    // Sample the texture at normalized UV coordinates (0 to 1)
    glm::vec3 sample(float u, float v) const 
    {
        // Clamp the coordinates to avoid out of bounds access
        u = glm::clamp(u, 0.0f, 1.0f);
        v = glm::clamp(v, 0.0f, 1.0f);

        // Convert UV to pixel coordinates
        int x = static_cast<int>(u * (width - 1));
        int y = static_cast<int>(v * (height - 1));

        // Calculate the index in the raw image data
        int index = (y * width + x) * components;

        // Fetch the color values (assuming RGB or RGBA format)
        if (components == 3) {
            return glm::vec3(data[index] / 255.0f, data[index + 1] / 255.0f, data[index + 2] / 255.0f);
        } else if (components == 4) {
            return glm::vec3(data[index] / 255.0f, data[index + 1] / 255.0f, data[index + 2] / 255.0f);
        }

        // Default to white if texture format is not supported
        return glm::vec3(1.0f);
    }

    
    int width, height, components;
    std::vector<uint8_t> data;
};

#endif // TEXTURE_H
