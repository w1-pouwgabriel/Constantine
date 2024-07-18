#include "Buffers.h"

namespace VkUtil
{
    void CopyBuffer(BufferBundleIn vkObjects, VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size) 
    {
        VkCommandBufferAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        allocInfo.commandPool = vkObjects.commandPool;
        allocInfo.commandBufferCount = 1;

        VkCommandBuffer commandBuffer;
        vkAllocateCommandBuffers(vkObjects.device, &allocInfo, &commandBuffer);

        VkCommandBufferBeginInfo beginInfo{};
        beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

        vkBeginCommandBuffer(commandBuffer, &beginInfo);

            VkBufferCopy copyRegion{};
            copyRegion.size = size;
            vkCmdCopyBuffer(commandBuffer, srcBuffer, dstBuffer, 1, &copyRegion);

        vkEndCommandBuffer(commandBuffer);

        VkSubmitInfo submitInfo{};
        submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
        submitInfo.commandBufferCount = 1;
        submitInfo.pCommandBuffers = &commandBuffer;

        vkQueueSubmit(vkObjects.graphicsQueue, 1, &submitInfo, VK_NULL_HANDLE);
        vkQueueWaitIdle(vkObjects.graphicsQueue);

        vkFreeCommandBuffers(vkObjects.device, vkObjects.commandPool, 1, &commandBuffer);
    }

    uint32_t FindMemoryType(const VkPhysicalDevice& physicalDevice, uint32_t typeFilter, VkMemoryPropertyFlags properties) 
    {
        VkPhysicalDeviceMemoryProperties memProperties;
        vkGetPhysicalDeviceMemoryProperties(physicalDevice, &memProperties);

        for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++) {
            if ((typeFilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties) {
                return i;
            }
        }

        throw std::runtime_error("failed to find suitable memory type!");
    }

    BufferBundleOut CreateBufferBundle(BufferBundleIn vkObjects, VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties)
    {
        BufferBundleOut output;

        VkBufferCreateInfo bufferInfo{};
        bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
        bufferInfo.size = size;
        bufferInfo.usage = usage;
        bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

        if (vkCreateBuffer(vkObjects.device, &bufferInfo, nullptr, &output.buffer) != VK_SUCCESS) {
            throw std::runtime_error("failed to create buffer!");
        }

        VkMemoryRequirements memRequirements;
        vkGetBufferMemoryRequirements(vkObjects.device, output.buffer, &memRequirements);

        VkMemoryAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
        allocInfo.allocationSize = memRequirements.size;
        allocInfo.memoryTypeIndex = FindMemoryType(vkObjects.physicalDevice, memRequirements.memoryTypeBits, properties);

        if (vkAllocateMemory(vkObjects.device, &allocInfo, nullptr, &output.bufferMemory) != VK_SUCCESS) {
            throw std::runtime_error("failed to allocate buffer memory!");
        }

        vkBindBufferMemory(vkObjects.device, output.buffer, output.bufferMemory, 0);

        return output;
    }

    BufferBundleOut CreateVertexBufferBundle(BufferBundleIn vkObjects, const std::vector<Vertex> &vertices)
    {
        VkDeviceSize bufferSize = sizeof(vertices[0]) * vertices.size();
        BufferBundleOut stagingBundle = CreateBufferBundle(vkObjects, bufferSize, 
            VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
            VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT 
            //VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT
        );

        void* data;
        vkMapMemory(vkObjects.device, stagingBundle.bufferMemory, 0, bufferSize, 0, &data);
            memcpy(data, vertices.data(), (size_t) bufferSize);
        vkUnmapMemory(vkObjects.device, stagingBundle.bufferMemory);

        BufferBundleOut output = CreateBufferBundle(vkObjects, bufferSize, 
            VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, 
            VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT
            //VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT
        );

        CopyBuffer(vkObjects, stagingBundle.buffer, output.buffer, bufferSize);

        vkDestroyBuffer(vkObjects.device, stagingBundle.buffer, nullptr);
        vkFreeMemory(vkObjects.device, stagingBundle.bufferMemory, nullptr);

        return output;
    }
}
