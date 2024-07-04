#pragma once
#include "Utils.h"
 
namespace VkInit
{
	const std::vector<const char*> deviceExtensions = {
        VK_KHR_SWAPCHAIN_EXTENSION_NAME
    };
	
	/**
		Choose a physical device based on the best hardware

		\param instance the vulkan instance to use
		\returns the chosen physical device
	*/
	VkPhysicalDevice MakePhysicalDevice(const VkInstance& instance, const VkSurfaceKHR &surface);

	/**
		Create a logical device based on the physical device

		\param physicalDevice the physical device that we are using
		\param surface the surface is needed to check for queue families
		\returns the logical device
	*/
	VkDevice MakeLogicalDevice(const VkPhysicalDevice& physicalDevice, const VkSurfaceKHR& surface);
}