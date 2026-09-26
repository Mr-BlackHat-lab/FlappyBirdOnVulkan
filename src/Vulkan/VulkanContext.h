#pragma once
#include "../VulkanState.hpp"
namespace VulkanContext {
    // Creates the Instance, sets up Validation Layers,create surface, picks the GPU, and creates the Logical Device
    void init(VulkanState& state);

    // Destroys the logical device, debug messenger, and instance in the correct order
    void cleanup(VulkanState& state);
}