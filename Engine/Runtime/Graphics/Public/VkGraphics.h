#pragma once
#include "Utils.h"
#include "Frame.h"
#include "Scene.h"

class VkGraphics
{
public:
	VkGraphics();
	VkGraphics(GLFWwindow* window, int width = 640, int height = 480);
	~VkGraphics();

	void Render(Scene& scene);
	void RecreateSwapchain();
	
	bool framebufferResized = false;
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
	VkQueue graphicsQueue = VK_NULL_HANDLE;
	VkQueue presentQueue = VK_NULL_HANDLE;
	void CreateDevice();

	// Swapchain
	VkSwapchainKHR swapchain = VK_NULL_HANDLE;
	std::vector<VkUtil::SwapChainFrame> swapchainFrames;
	VkFormat swapchainFormat;
	VkExtent2D swapchainExtent;
	void CreateSwapchain();
	void CleanupSwapchain();
	void CreateFramebuffers();
	
	//	Pipeline creation
	VkPipelineLayout layout = VK_NULL_HANDLE;
	VkRenderPass renderPass = VK_NULL_HANDLE;
	VkPipeline pipeline = VK_NULL_HANDLE;
	void CreateGraphicsPipeline(const std::string& vertexFilepath, const std::string& fragmentFilepath);

	//	Command-related variables
	VkCommandPool commandPool = VK_NULL_HANDLE;
	VkCommandBuffer mainCommandBuffer = VK_NULL_HANDLE;
	int maxFramesInFlight, frameNumber = 0;
	void CreatCommandPool();
	void CreateSyncObjects();

	void FinalizeSetup();

	// TEMP
	VkUtil::Model model;

	//Draw command buffers
	void DrawCommandbuffer(VkCommandBuffer commandBuffer, int32_t imageIndex, Scene& scene);
};

static void framebufferResizeCallback(GLFWwindow* window, int width, int height) {
	auto app = reinterpret_cast<VkGraphics*>(glfwGetWindowUserPointer(window));
    app->framebufferResized = true;
}