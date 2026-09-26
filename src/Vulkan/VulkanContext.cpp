#include "VulkanContext.h"
#include <stdexcept>
#include <iostream>
#include <vector>

namespace VulkanContext {
    //helper function not exposed in header
    static void createInstace(VulkanState& state) {

        //AppInfo for Vulkan
        VkApplicationInfo appInfo{};
        appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
        appInfo.pApplicationName = "FlappyBird";
        appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
        appInfo.pEngineName = "FlappyEngine";
        appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
        appInfo.apiVersion = VK_API_VERSION_1_0;

        VkInstanceCreateInfo instanceCreateInfo{};
        instanceCreateInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
        instanceCreateInfo.pApplicationInfo = &appInfo;

        // Ask GLFW for the base extensions it needs for the current OS (Windows/Linux/Mac)
        uint32_t glfwExtensionCount = 0;
        const char** glfwExtensions= glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

        std::vector<const char*> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);

        // Add your custom debug extension to the list
        // extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);

        const char* validationLayers[]={
            "VK_LAYER_KHRONOS_validation",
            // "VK_LAYER_LUNARG_api_dumps", //log vulkan api calls
        };

        instanceCreateInfo.enabledLayerCount = static_cast<uint32_t>(std::size(validationLayers));
        instanceCreateInfo.ppEnabledLayerNames = validationLayers;

        instanceCreateInfo.enabledExtensionCount = static_cast<uint32_t>(std::size(extensions));
        instanceCreateInfo.ppEnabledExtensionNames = extensions.data();

        VkResult result = vkCreateInstance(&instanceCreateInfo, nullptr, &state.instance);

        if (result != VK_SUCCESS) {
            std::cout << "Failed to create Vulkan instance."
                      << std::endl;
            throw std::runtime_error("Failed to create Vulkan instance.");
        }
        std::cout << "Vulkan instance created successfully."
                  << std::endl;
    }
    void init(VulkanState& state) {
        createInstace(state);
    }
    void cleanup(VulkanState& state) {
        if (state.instance != VK_NULL_HANDLE) {
            vkDestroyInstance(state.instance, nullptr);
            std::cout << "Vulkan instance destroyed successfully.\nK";
        }
    }
}
