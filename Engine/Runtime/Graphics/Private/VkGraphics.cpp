#include "VkGraphics.h"
#include "Instance.h"
#include "Logger.h"
#include "Device.h"
#include "QueueFamilies.h"
#include "Swapchain.h"
#include "Pipeline.h"
#include "Scene.h"
#include "Command.h"
#include "Sync.h"
#include "Frame.h"

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
	CreatCommandPool();
	CreateFramebuffers();
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
	VkUtil::SwapChainBundle swapchainBundle = VkUtil::CreateSwapchainKHR(physicalDevice, device, surface, window);
	swapchain = swapchainBundle.swapchain;
	swapchainExtent = swapchainBundle.extent;
	swapchainFormat = swapchainBundle.format;
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
	renderPass = graphisBundle.renderPass;
	layout = graphisBundle.layout;
	pipeline = graphisBundle.pipeline;
}

void VkGraphics::CreatCommandPool()
{
	VkUtil::QueueFamilyIndices queueFamilyIndices = VkUtil::findQueueFamilies(physicalDevice, surface);
	VkCommandPoolCreateInfo poolInfo{};
	poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
	poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
	poolInfo.queueFamilyIndex = queueFamilyIndices.graphicsFamily.value();

	if (vkCreateCommandPool(device, &poolInfo, nullptr, &commandPool) != VK_SUCCESS) {
		throw std::runtime_error("failed to create command pool!");
	}
}

void VkGraphics::FinalizeSetup()
{
	for (size_t i = 0; i < swapchainFrames.size(); i++)
	{
		auto& frame = swapchainFrames[i];
		VkInit::AddCommandBuffer(device, commandPool, frame);
		VkInit::AddSyncObjects(device, frame);
	}
}

void VkGraphics::CreateFramebuffers()
{
	// VkCommandBufferAllocateInfo allocInfo{};
	// allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	// allocInfo.commandPool = commandPool;
	// allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	// allocInfo.commandBufferCount = 1;

	// if (vkAllocateCommandBuffers(device, &allocInfo, &mainCommandBuffer) != VK_SUCCESS) {
	// 	throw std::runtime_error("failed to allocate command buffers!");
	// }

	VkUtil::SwapChainBundle swapchainBundle = {swapchain, swapchainFrames, swapchainFormat, swapchainExtent};
	VkUtil::InitImageAndFrameData(device, swapchainBundle, renderPass, commandPool);
	swapchainFrames = swapchainBundle.frames;
	maxFramesInFlight = swapchainBundle.imageCount - 1;
	frameNumber = 0;
}

void VkGraphics::DrawCommandbuffer(VkCommandBuffer commandBuffer, int32_t imageIndex, Scene& scene) 
{
	VkCommandBufferBeginInfo beginInfo{};
	beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

	if (vkBeginCommandBuffer(commandBuffer, &beginInfo) != VK_SUCCESS) {
		throw std::runtime_error("failed to begin recording command buffer!");
	}

	VkRenderPassBeginInfo renderPassInfo{};
	renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
	renderPassInfo.renderPass = renderPass;
	renderPassInfo.framebuffer = swapchainFrames[imageIndex].frameBuffer;
	renderPassInfo.renderArea.offset = {0, 0};
	renderPassInfo.renderArea.extent = swapchainExtent;

	VkClearValue clearColor = {{{0.0f, 0.0f, 0.0f, 1.0f}}};
	renderPassInfo.clearValueCount = 1;
	renderPassInfo.pClearValues = &clearColor;

	vkCmdBeginRenderPass(commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

		vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline);

		VkViewport viewport{};
		viewport.x = 0.0f;
		viewport.y = 0.0f;
		viewport.width = (float) swapchainExtent.width;
		viewport.height = (float) swapchainExtent.height;
		viewport.minDepth = 0.0f;
		viewport.maxDepth = 1.0f;
		vkCmdSetViewport(commandBuffer, 0, 1, &viewport);

		VkRect2D scissor{};
		scissor.offset = {0, 0};
		scissor.extent = swapchainExtent;
		vkCmdSetScissor(commandBuffer, 0, 1, &scissor);            

		vkCmdDraw(commandBuffer, 3, 1, 0, 0);

	vkCmdEndRenderPass(commandBuffer);

	if (vkEndCommandBuffer(commandBuffer) != VK_SUCCESS) {
		throw std::runtime_error("failed to record command buffer!");
	}
}

void VkGraphics::RecreateSwapchain()
{	
	vkDeviceWaitIdle(device);

    CleanupSwapchain();

	CreateSwapchain();
	CreateFramebuffers();
}

void VkGraphics::CleanupSwapchain()
{
	for (size_t i = 0; i < swapchainFrames.size(); i++)
	{
		vkDestroyFramebuffer(device, swapchainFrames[i].frameBuffer, nullptr);
		swapchainFrames[i].frameBuffer = VK_NULL_HANDLE;
		vkDestroyImageView(device, swapchainFrames[i].imageView, nullptr);
		swapchainFrames[i].imageView = VK_NULL_HANDLE;
	}
	vkDestroySwapchainKHR(device, swapchain, nullptr);
}

void VkGraphics::Render(Scene& scene)
{
	auto& frame = swapchainFrames[frameNumber];

	vkWaitForFences(device, 1, &frame.inFlight, VK_TRUE, UINT64_MAX);
	vkResetFences(device, 1, &frame.inFlight);
	
	uint32_t imageIndex;
	VkResult result = vkAcquireNextImageKHR(device, swapchain, UINT64_MAX, frame.imageAvailable, VK_NULL_HANDLE, &imageIndex);

	if (result == VK_ERROR_OUT_OF_DATE_KHR) {
		RecreateSwapchain();
		return;
	} else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
		throw std::runtime_error("failed to acquire swap chain image!");
	}

	vkResetCommandBuffer(frame.commandBuffer, /*VkCommandBufferResetFlagBits*/ 0);
	DrawCommandbuffer(frame.commandBuffer, imageIndex, scene);

	VkSubmitInfo submitInfo{};
	submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

	VkSemaphore waitSemaphores[] = {frame.imageAvailable};
	VkPipelineStageFlags waitStages[] = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
	submitInfo.waitSemaphoreCount = 1;
	submitInfo.pWaitSemaphores = waitSemaphores;
	submitInfo.pWaitDstStageMask = waitStages;

	submitInfo.commandBufferCount = 1;
	submitInfo.pCommandBuffers = &frame.commandBuffer;

	VkSemaphore signalSemaphores[] = {frame.renderFinished};
	submitInfo.signalSemaphoreCount = 1;
	submitInfo.pSignalSemaphores = signalSemaphores;

	if (vkQueueSubmit(graphicsQueue, 1, &submitInfo, frame.inFlight) != VK_SUCCESS) {
		throw std::runtime_error("failed to submit draw command buffer!");
	}

	VkPresentInfoKHR presentInfo{};
	presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
	presentInfo.waitSemaphoreCount = 1;
	presentInfo.pWaitSemaphores = signalSemaphores;

	VkSwapchainKHR swapChains[] = {swapchain};
	presentInfo.swapchainCount = 1;
	presentInfo.pSwapchains = swapChains;
	presentInfo.pImageIndices = &imageIndex;

	vkQueuePresentKHR(presentQueue, &presentInfo);

	frameNumber = (frameNumber + 1) % maxFramesInFlight;
}

VkGraphics::~VkGraphics()
{
	vkDestroyCommandPool(device, commandPool, nullptr);
	vkDestroyPipeline(device, pipeline, nullptr);
	vkDestroyPipelineLayout(device, layout, nullptr);
	vkDestroyRenderPass(device, renderPass, nullptr);

    for (size_t i = 0; i < swapchainFrames.size(); i++)
	{
		vkDestroyFramebuffer(device, swapchainFrames[i].frameBuffer, nullptr);
		swapchainFrames[i].frameBuffer = VK_NULL_HANDLE;
		vkDestroyImageView(device, swapchainFrames[i].imageView, nullptr);
		swapchainFrames[i].imageView = VK_NULL_HANDLE;
		vkDestroySemaphore(device, swapchainFrames[i].imageAvailable, nullptr);
		swapchainFrames[i].imageAvailable = VK_NULL_HANDLE;
		vkDestroySemaphore(device, swapchainFrames[i].renderFinished, nullptr);
		swapchainFrames[i].renderFinished = VK_NULL_HANDLE;
		vkDestroyFence(device, swapchainFrames[i].inFlight, nullptr);
		swapchainFrames[i].inFlight = VK_NULL_HANDLE;
	}
	vkDestroySwapchainKHR(device, swapchain, nullptr);
	vkDestroySurfaceKHR(instance, surface, nullptr);
    vkDestroyDevice(device, nullptr);

	#ifdef ENABLE_VALIDATION_LAYER
		VkUtil::DetachDebugUtilsMessengerEXT(instance, debugMessenger, nullptr);
	#endif
	vkDestroyInstance(instance, nullptr);
}