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
	
}