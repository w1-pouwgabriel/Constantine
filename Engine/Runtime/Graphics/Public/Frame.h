#pragma once
#include "Utils.h"

namespace VkUtil{
    /**
		Store all the image information
	*/
	struct SwapChainFrame{
		VkImage image;
		VkImageView imageView;
		VkFramebuffer frameBuffer;
		VkCommandBuffer commandBuffer;
		//Synchronization objects
		VkFence inFlight;
		VkSemaphore imageAvailable, renderFinished;
	};

	
	struct SwapChainBundle {
		VkSwapchainKHR swapchain;
		std::vector<VkUtil::SwapChainFrame> frames;
		VkFormat format;
		VkExtent2D extent;
		uint32_t imageCount;
	};
	
	void InitImageAndFrameData(const VkDevice &device, SwapChainBundle& swapchainBundle, const VkRenderPass& renderPass, const VkCommandPool& commandPool);
}