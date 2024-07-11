#pragma once
#include "Utils.h"
#include "Frame.h"
 
namespace VkUtil
{
    struct SwapChainSupportDetails {
        VkSurfaceCapabilitiesKHR capabilities;
        std::vector<VkSurfaceFormatKHR> formats;
        std::vector<VkPresentModeKHR> presentModes;
    };

    SwapChainSupportDetails QuerySwapChainSupport(const VkPhysicalDevice& physicalDevice, const VkSurfaceKHR& surface);

    SwapChainBundle CreateSwapchainKHR(const VkPhysicalDevice &physicalDevice, const VkDevice &device, const VkSurfaceKHR &surface, GLFWwindow *window);

    void AddSwapchainBundleFrames(const VkDevice &device, SwapChainBundle& swapchainBundle, const VkRenderPass& renderPass, const VkCommandPool& commandPool);
}