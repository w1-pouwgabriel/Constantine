#pragma once
#include "Utils.h"
#include "Frame.h"
 
namespace VkInit
{
	void AddCommandBuffer(const VkDevice &device, const VkCommandPool& commandPool, VkUtil::SwapChainFrame& frame);
}