#pragma once

#include "Utils.h"
#include "Shader.h"

namespace VkUtil
{
    const std::vector<VkDynamicState> dynamicStates = {
        VK_DYNAMIC_STATE_VIEWPORT,
        VK_DYNAMIC_STATE_SCISSOR
    };

	struct GraphicsPipelineInBundle {
		VkDevice device;
		std::string vertexFilepath;
		std::string fragmentFilepath;
		VkExtent2D swapchainExtent;
		VkFormat swapchainImageFormat;
	};

	struct GraphicsPipelineOutBundle {
		VkPipelineLayout layout;
		VkRenderPass renderPass;
		VkPipeline pipeline;
	};

    /**
		Make a graphics pipeline, along with renderpass and pipeline layout

		\param specification the struct holding input data
		\returns the bundle of data structures created
	*/
	GraphicsPipelineOutBundle CreateGraphicsPipeline(GraphicsPipelineInBundle& specification);

} // namespace VkUtil
