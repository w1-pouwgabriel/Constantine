#pragma once
#include "Utils.h"
#include "Frame.h"
 
namespace VkInit
{
    void AddSyncObjects(const VkDevice &device, VkUtil::SwapChainFrame& frame);
}