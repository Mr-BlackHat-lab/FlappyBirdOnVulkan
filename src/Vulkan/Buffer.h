#pragma once

#include "../VulkanState.hpp"

namespace Buffer {
    void createFramebuffers(VulkanState& state);
    void createCommandPool(VulkanState& state);
    void createCommandBuffers(VulkanState& state);

    void destroyFramebuffers(VulkanState& state);
    void destroyCommandPool(VulkanState& state);

    void init(VulkanState& state);
    void cleanup(VulkanState& state);
}