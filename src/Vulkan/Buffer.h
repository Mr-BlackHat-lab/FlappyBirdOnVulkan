#pragma once

#include "../VulkanState.hpp"

namespace Buffer {
    void createFramebuffers(VulkanState& state);

    void destroyFramebuffers(VulkanState& state);
}