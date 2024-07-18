#include "Model.h"
#include "Buffers.h"

namespace VkUtil
{
    Model::Model()
    {
        vertices.resize(0);
        indices.resize(0);
        vertexBuffer = VK_NULL_HANDLE;
        vertexBufferMemory = VK_NULL_HANDLE;
    }
}