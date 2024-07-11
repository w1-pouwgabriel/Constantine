#include "Frame.h"
#include "Swapchain.h"

namespace VkUtil
{
    void InitImageAndFrameData(const VkDevice &device, SwapChainBundle& swapchainBundle, const VkRenderPass& renderPass, const VkCommandPool& commandPool)
    {
        vkGetSwapchainImagesKHR(device, swapchainBundle.swapchain, &swapchainBundle.imageCount, nullptr);
        swapchainBundle.frames.resize(swapchainBundle.imageCount);

        std::vector<VkImage> swapChainImages;
        swapChainImages.resize(swapchainBundle.imageCount);
        vkGetSwapchainImagesKHR(device, swapchainBundle.swapchain, &swapchainBundle.imageCount, swapChainImages.data());

        for(int i = 0; i < swapchainBundle.imageCount; i++)
        {
            VkUtil::SwapChainFrame& currentFrame = swapchainBundle.frames[i];
            currentFrame.image = swapChainImages[i];

            VkImageViewCreateInfo createInfo{};
            createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
            createInfo.image = currentFrame.image;
            createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
            createInfo.format = swapchainBundle.format;
            createInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
            createInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
            createInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
            createInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
            createInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
            createInfo.subresourceRange.baseMipLevel = 0;
            createInfo.subresourceRange.levelCount = 1;
            createInfo.subresourceRange.baseArrayLayer = 0;
            createInfo.subresourceRange.layerCount = 1;

            if (vkCreateImageView(device, &createInfo, nullptr, &currentFrame.imageView) != VK_SUCCESS) {
                throw std::runtime_error("failed to create image views!");
            }

            VkFramebufferCreateInfo framebufferInfo{};
            framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
            framebufferInfo.renderPass = renderPass;
            framebufferInfo.attachmentCount = 1;
            framebufferInfo.pAttachments = &currentFrame.imageView;
            framebufferInfo.width = swapchainBundle.extent.width;
            framebufferInfo.height = swapchainBundle.extent.height;
            framebufferInfo.layers = 1;

            if (vkCreateFramebuffer(device, &framebufferInfo, nullptr, &currentFrame.frameBuffer) != VK_SUCCESS) {
                throw std::runtime_error("failed to create framebuffer!");
            }
        }
    }
} // namespace VkUtil
