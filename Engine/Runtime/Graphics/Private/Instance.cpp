#include "Instance.h"
#include "Logger.h"

namespace VkInit
{
    std::vector<const char*> getRequiredExtensions() 
    {
        uint32_t glfwExtensionCount = 0;
        const char** glfwExtensions;
        glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

        std::vector<const char*> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);

        #ifdef ENABLE_VALIDATION_LAYER
            extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
        #endif

        return extensions;
    }

    bool supported(std::vector<const char*>& extensions, std::vector<const char*>& layers)
    {
        return false;
    }

    VkInstance MakeInstance(const char* applicationName)
    {
        //Setup validation layers
        #ifdef ENABLE_VALIDATION_LAYER
            uint32_t layerCount;
            vkEnumerateInstanceLayerProperties(&layerCount, nullptr);
            std::vector<VkLayerProperties> availableLayers(layerCount);
            vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

            for (const char* layerName : validationLayers) {
                bool layerFound = false;
                //std::cout << "available layers:\n";
                for (const auto& layerProperties : availableLayers) {
                    //std::cout << '\t' << layerProperties.layerName << '\n';
                    if (strcmp(layerName, layerProperties.layerName) == 0) {
                        layerFound = true;
                        break;
                    }
                }
                //std::cout << "\n";
                if (!layerFound) {
                    throw std::runtime_error("validation layers requested, but not available!");
                }
            }
        #endif

        // VkApplicationInfo holds information about your application.
        VkApplicationInfo appInfo{};
        appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
        appInfo.pApplicationName = applicationName;
        appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
        appInfo.pEngineName = "Vulkan graphics engine";
        appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
        appInfo.apiVersion = VK_API_VERSION_1_0;

        // VkInstanceCreateInfo is used to specify details about the instance to create.
        VkInstanceCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
        createInfo.pApplicationInfo = &appInfo;

        // Get the required Vulkan extensions from GLFW
        uint32_t glfwExtensionCount = 0;
        const char** glfwExtensions;
        auto extensions = getRequiredExtensions();
        createInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
        createInfo.ppEnabledExtensionNames = extensions.data();
        VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo{};
        #ifdef ENABLE_VALIDATION_LAYER
            createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
            createInfo.ppEnabledLayerNames = validationLayers.data();
            
            VkUtil::populateDebugMessengerCreateInfo(debugCreateInfo);
            createInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT*) &debugCreateInfo;
        #else 
            createInfo.enabledLayerCount = 0;
        #endif

        VkInstance instance;
        try
        {
            vkCreateInstance(&createInfo, nullptr, &instance);
            return instance;
        }
        catch(const std::system_error err)
        {
            std::cout << "Failed to create instance: " << err.what() << "\n";
            return nullptr;
        }
    }
}