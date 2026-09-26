#include "VulkanContext.h"
#include <stdexcept>
#include <iostream>
#include <optional>
#include <set>
#include <vector>

namespace VulkanContext {
    //helper function not exposed in header

    //instace creation
    static void createInstance(VulkanState& state) {

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
    //crating surface
    static void createSurface(VulkanState& state) {
        // glfwCreateWindowSurface takes the Vulkan instance, the GLFW window,
        // an optional allocator, and a pointer to the surface variable.
        VkResult result= glfwCreateWindowSurface(state.instance, state.window, nullptr, &state.surface);

        if (result != VK_SUCCESS) {
            throw std::runtime_error("Failed to create window surface!");
        }
        printf("Window Surface created successfully.\n");
    }

    // selecting physical device
    struct QueueFamilyIndices {
        std::optional<uint32_t> graphicsFamily;
        std::optional<uint32_t> presentFamily;

        bool isComplete() {
            return graphicsFamily.has_value() && presentFamily.has_value();
        }
    };
    static QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device, VkSurfaceKHR surface) { // <-- Pass device and surface
        QueueFamilyIndices indices;

        uint32_t queueFamilyCount = 0;
        vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr); // <-- Use device
        std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
        vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data()); // <-- Use device

        int i = 0;
        for (const auto& queueFamily : queueFamilies) {
            if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
                indices.graphicsFamily = i;
            }

            VkBool32 presentSupport = false;
            vkGetPhysicalDeviceSurfaceSupportKHR(device, i, surface, &presentSupport); // <-- Use device and surface
            if (presentSupport) {
                indices.presentFamily = i;
            }
            if (indices.isComplete()) {
                break;
            }
            i++;
        }
        return indices;
    }

    bool isDeviceSuitable(VkPhysicalDevice device, VkSurfaceKHR surface) { // <-- Pass device and surface
        QueueFamilyIndices indices = findQueueFamilies(device, surface);
        return indices.isComplete();
    }
    int rateDeviceSuitability(VkPhysicalDevice device, VkSurfaceKHR surface) {
        // If it doesn't have the required queues, it's completely unsuitable (Score = 0)
        if (!isDeviceSuitable(device, surface)) {
            return 0;
        }

        VkPhysicalDeviceProperties deviceProperties;
        vkGetPhysicalDeviceProperties(device, &deviceProperties);

        int score = 0;

        // Discrete GPUs (Dedicated Graphics Cards like NVIDIA RTX or AMD Radeon)
        // have a massive performance advantage over integrated ones.
        if (deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU) {
            score += 1000;
        }
        // Integrated GPUs (like Intel UHD or AMD Radeon Graphics) are fallback options
        else if (deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU) {
            score += 100;
        }

        // Add the maximum texture size as a minor score boost to differentiate
        // between two GPUs of the same type (the one with higher limits wins)
        score += deviceProperties.limits.maxImageDimension2D;

        return score;
    }
    static void pickPhysicalDevice(VulkanState& state) {
        uint32_t deviceCount = 0;
        vkEnumeratePhysicalDevices(state.instance, &deviceCount, nullptr);

        if (deviceCount == 0) {
            throw std::runtime_error("No Vulkan devices found.");
        }
        std::vector<VkPhysicalDevice> devices(deviceCount);
        vkEnumeratePhysicalDevices(state.instance, &deviceCount, devices.data());

        int highestScore = -1;

        // Evaluate every device and pick the one with the highest score
        for (const auto& device : devices) {
            int score = rateDeviceSuitability(device, state.surface);

            if (score > highestScore) {
                highestScore = score;
                state.physicalDevice = device;
            }
        }

        // If the highest score is 0, no devices had the required queues
        if (highestScore == 0 || state.physicalDevice == VK_NULL_HANDLE) {
            throw std::runtime_error("Failed to find a suitable Vulkan device.");
        }

        // Print the name of the selected GPU to verify
        VkPhysicalDeviceProperties deviceProperties;
        vkGetPhysicalDeviceProperties(state.physicalDevice, &deviceProperties);
        printf("Selected GPU: %s (Score: %d)\n", deviceProperties.deviceName, highestScore);
    }
    static void createLogicalDevice(VulkanState& state) {
        QueueFamilyIndices indices = findQueueFamilies(state.physicalDevice, state.surface);

        // I used a set to ensure i don't request the same queue family twice
        // if graphics and present are the same index.
        std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
        std::set<uint32_t> uinqueQueueFamilies={
            indices.graphicsFamily.value(),
            indices.presentFamily.value()
        };

        float queuePriority = 1.0f;
        for (uint32_t queueFamily : uinqueQueueFamilies) {
            VkDeviceQueueCreateInfo queueCreateInfo = {};
            queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
            queueCreateInfo.queueFamilyIndex = queueFamily;
            queueCreateInfo.queueCount = 1;
            queueCreateInfo.pQueuePriorities = &queuePriority;
            queueCreateInfos.push_back(queueCreateInfo);
        }

        VkPhysicalDeviceFeatures deviceFeatures = {};

        VkDeviceCreateInfo createInfo = {};
        createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;

        createInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
        createInfo.pQueueCreateInfos = queueCreateInfos.data();

        createInfo.pEnabledFeatures = &deviceFeatures;

        const std::vector<const char*> deviceExtensions = {VK_KHR_SWAPCHAIN_EXTENSION_NAME};

        createInfo.enabledExtensionCount = static_cast<uint32_t>(deviceExtensions.size());
        createInfo.ppEnabledExtensionNames = deviceExtensions.data();

        createInfo.enabledLayerCount = 0;

        // Actual create of Logical Device
        VkResult result = vkCreateDevice(state.physicalDevice, &createInfo, nullptr, &state.device);
        if (result != VK_SUCCESS) {
            throw std::runtime_error("Failed to create logical device.");
        }

        vkGetDeviceQueue(state.device, indices.graphicsFamily.value(), 0, &state.graphicsQueue);
        vkGetDeviceQueue(state.device, indices.presentFamily.value(), 0, &state.presentQueue);

        printf("Logical Device created and queues retrieved.\n");

    }
    void init(VulkanState& state) {
        createInstance(state);
        createSurface(state);
        pickPhysicalDevice(state);
        createLogicalDevice(state);
    }
    void cleanup(VulkanState& state) {
        if (state.device != VK_NULL_HANDLE) {
            vkDestroyDevice(state.device, nullptr);
            std::cout << "Vulkan logical Device destroyed successfully.\n";
        }
        if (state.surface != VK_NULL_HANDLE) {
            vkDestroySurfaceKHR(state.instance, state.surface, nullptr);
            std::cout << "Vulkan surface destroyed successfully.\n";
        }
        if (state.instance != VK_NULL_HANDLE) {
            vkDestroyInstance(state.instance, nullptr);
            std::cout << "Vulkan instance destroyed successfully.\n";
        }
    }
}
