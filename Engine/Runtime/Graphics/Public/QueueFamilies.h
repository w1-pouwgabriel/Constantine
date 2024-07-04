#pragma once

#include "Utils.h"

namespace VkUtil
{
	struct QueueFamilyIndices {
		std::optional<uint32_t> graphicsFamily;
		std::optional<uint32_t> presentFamily;

		bool isComplete() {
			return graphicsFamily.has_value() && presentFamily.has_value();
		}
	};
    	
    /**
		Check whether the given physical device has de needed families

		\param device the physical device to check.
		\param surface check for present queue support
		\returns a struct of QueueFamilyIndices
	*/
	QueueFamilyIndices findQueueFamilies(const VkPhysicalDevice& device, const VkSurfaceKHR &surface);
}