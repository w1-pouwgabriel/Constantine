#include "VkGraphics.h"
#include "Instance.h"
#include "Logger.h"
#include "Device.h"
#include "QueueFamilies.h"
#include "Swapchain.h"
#include "Pipeline.h"
#include "Scene.h"

#include <glm/ext/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale

VkGraphics::VkGraphics()
{
	//Empty for now
}

VkGraphics::VkGraphics(int width, int height, GLFWwindow *window)
{
	windowWidth = width;
	windowHeight = height;
	this->window = window;

	CreateInstance();
	CreateSurface(window);
	CreateDevice();
	CreateSwapchain();
	CreateGraphicsPipeline("Shaders/vert.spv", "Shaders/frag.spv");
	FinalizeSetup();
} 

void VkGraphics::CreateInstance()
{
	instance = VkInit::MakeInstance("Graphics engine");
	VkUtil::AttachDebugUtilsMessengerEXT(instance, nullptr, &debugMessenger);
}


void VkGraphics::CreateSurface(GLFWwindow *window)
{
	if (glfwCreateWindowSurface(instance, window, nullptr, &surface) != VK_SUCCESS) {
        throw std::runtime_error("failed to create window surface!");
    }
}

void VkGraphics::CreateDevice()
{
	physicalDevice = VkInit::MakePhysicalDevice(instance, surface);
	device = VkInit::MakeLogicalDevice(physicalDevice, surface);

	VkUtil::QueueFamilyIndices indices = VkUtil::findQueueFamilies(physicalDevice, surface);
	vkGetDeviceQueue(device, indices.graphicsFamily.value(), 0, &graphicsQueue);
	vkGetDeviceQueue(device, indices.presentFamily.value(), 0, &presentQueue);
}

void VkGraphics::CreateSwapchain()
{
	VkInit::SwapChainBundle swapchainBundle = VkInit::CreateSwapchainKHR(physicalDevice, device, surface, window);
	swapchain = swapchainBundle.swapchain;
	swapchainExtent = swapchainBundle.extent;
	swapchainFormat = swapchainBundle.format;
	VkInit::AddSwapchainBundleFrames(device, swapchainBundle);
	swapchainFrames = swapchainBundle.frames;
}

void VkGraphics::CreateGraphicsPipeline(const std::string& vertexFilepath, const std::string& fragmentFilepath)
{
	VkUtil::GraphicsPipelineInBundle input;
	input.device = device;
	input.vertexFilepath = vertexFilepath;
	input.fragmentFilepath = fragmentFilepath;
	input.swapchainExtent = swapchainExtent;
	input.swapchainImageFormat = swapchainFormat;

	VkUtil::GraphicsPipelineOutBundle graphisBundle = VkUtil::CreateGraphicsPipeline(input);

}

void VkGraphics::RecreateSwapchain()
{
	
}

void VkGraphics::CreateFramebuffers() 
{
	
}

void VkGraphics::CleanupSwapchain()
{
	
}

void VkGraphics::CreateSyncObjects()
{
	
}

void VkGraphics::FinalizeSetup()
{
	
}

void VkGraphics::DrawCommandbuffer(VkCommandBuffer commandBuffer, int32_t imageIndex, Scene& scene) 
{
	
}

void VkGraphics::Render(Scene& scene)
{
	int test = 0;
}

void VkGraphics::Cleanup()
{
    for (size_t i = 0; i < swapchainFrames.size(); i++)
	{
		vkDestroyImageView(device, swapchainFrames[i].imageView, nullptr);
		swapchainFrames[i].imageView = VK_NULL_HANDLE;
	}
	vkDestroySwapchainKHR(device, swapchain, nullptr);
}

VkGraphics::~VkGraphics()
{
	Cleanup();

	vkDestroySurfaceKHR(instance, surface, nullptr);
    vkDestroyDevice(device, nullptr);
	#ifdef ENABLE_VALIDATION_LAYER
		VkUtil::DetachDebugUtilsMessengerEXT(instance, debugMessenger, nullptr);
	#endif
	vkDestroyInstance(instance, nullptr);
}