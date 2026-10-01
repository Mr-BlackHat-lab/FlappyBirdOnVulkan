#pragma once

#include "../VulkanState.hpp"

namespace Pipeline {
    void createRenderPass(VulkanState& state);

    void cleanup(VulkanState& state);
}