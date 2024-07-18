#pragma once

#include "Utils.h"
#include "Vertex.h"
 
namespace VkUtil
{
    struct BufferBundleIn
    {
        VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
        VkDevice device = VK_NULL_HANDLE; 
        VkCommandPool commandPool = VK_NULL_HANDLE;
        VkQueue graphicsQueue = VK_NULL_HANDLE;
    };

    struct BufferBundleOut
    {
        VkBuffer buffer = VK_NULL_HANDLE;
        VkDeviceMemory bufferMemory = VK_NULL_HANDLE;
    };

    BufferBundleOut CreateVertexBufferBundle(BufferBundleIn vkObjects, const std::vector<Vertex> &vertices);
}