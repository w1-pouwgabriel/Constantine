#pragma once
#include "Utils.h"
#include "Frame.h"
 
namespace VkInit
{
    struct SwapChainSupportDetails {
        VkSurfaceCapabilitiesKHR capabilities;
        std::vector<VkSurfaceFormatKHR> formats;
        std::vector<VkPresentModeKHR> presentModes;
    };

	struct SwapChainBundle {
		VkSwapchainKHR swapchain;
		std::vector<VkUtil::SwapChainFrame> frames;
		VkFormat format;
		VkExtent2D extent;
		uint32_t imageCount;
	};

    SwapChainSupportDetails QuerySwapChainSupport(const VkPhysicalDevice& physicalDevice, const VkSurfaceKHR& surface);

    SwapChainBundle CreateSwapchainKHR(const VkPhysicalDevice &physicalDevice, const VkDevice &device, const VkSurfaceKHR &surface, GLFWwindow *window);

    void AddSwapchainBundleFrames(const VkDevice &device, SwapChainBundle& swapchainBundle);
}