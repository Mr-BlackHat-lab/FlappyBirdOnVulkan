#pragma once
#include "../VulkanState.hpp"


const std::vector<const char*> deviceExtensions = {
    VK_KHR_SWAPCHAIN_EXTENSION_NAME
};


namespace VulkanContext {
    // Creates the Instance, sets up Validation Layers,create surface, picks the GPU, creates the Logical Device, call swapchain creater
    void init(VulkanState& state);

    // Destroys the logical device, debug messenger, and instance in the correct order
    void cleanup(VulkanState& state);
}