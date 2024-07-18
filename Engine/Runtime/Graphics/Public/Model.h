#pragma once
#include "Utils.h"
#include "Vertex.h"

namespace VkUtil
{
    struct Model
    {
        Model();

        std::vector<Vertex> vertices;
        std::vector<uint32_t> indices;
        VkBuffer vertexBuffer;
        VkDeviceMemory vertexBufferMemory;
    };

    
}