#pragma once
#include "Utils.h"
#include "Frame.h"
#include "Scene.h"

class VkGraphics
{
public:
	VkGraphics();
	VkGraphics(int width, int height, GLFWwindow* window);
	~VkGraphics();

	void Render(Scene& scene);
	void RecreateSwapchain();
	void Cleanup();

private:
	//GLFW
	int windowWidth;
	int windowHeight;
	GLFWwindow* window;

	//Vulkan
	//	Instance
	VkInstance instance;
	VkDebugUtilsMessengerEXT debugMessenger;
	VkSurfaceKHR surface;
	void CreateInstance();
	void CreateSurface(GLFWwindow* window);
	//	Device
	VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
	VkDevice device = VK_NULL_HANDLE;
	VkQueue graphicsQueue;
	VkQueue presentQueue;
	void CreateDevice();
	// Swapchain
	VkSwapchainKHR swapchain = VK_NULL_HANDLE;
	std::vector<VkUtil::SwapChainFrame> swapchainFrames;
	VkFormat swapchainFormat;
	VkExtent2D swapchainExtent;
	void CreateSwapchain();

	//Pipeline
	//vk::PipelineLayout layout;
	//vk::RenderPass renderpass;
	//vk::Pipeline pipeline;
	void CreatePipeline();
	void CleanupSwapchain();

	//Command-related variables
	//vk::CommandPool commandPool;
	//vk::CommandBuffer mainCommandBuffer;
	void CreateFramebuffers();

	//Synchronization objects
	int maxFramesInFlight, frameNumber;
	void CreateSyncObjects();

	void FinalizeSetup();

	//Draw command buffers
	void DrawCommandbuffer(VkCommandBuffer commandBuffer, int32_t imageIndex, Scene& scene);
};