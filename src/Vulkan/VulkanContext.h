#pragma once
#include <optional>

#include "../VulkanState.hpp"


const std::vector<const char*> deviceExtensions = {
    VK_KHR_SWAPCHAIN_EXTENSION_NAME
};
struct QueueFamilyIndices {
    std::optional<uint32_t> graphicsFamily;
    std::optional<uint32_t> presentFamily;

    bool isComplete() {
        return graphicsFamily.has_value() && presentFamily.has_value();
    }
};

namespace VulkanContext {

    QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device, VkSurfaceKHR surface);
    // Creates the Instance, sets up Validation Layers,create surface, picks the GPU, creates the Logical Device, call swapchain creater
    void init(VulkanState& state);

    // Destroys the logical device, debug messenger, and instance in the correct order
    void cleanup(VulkanState& state);
}