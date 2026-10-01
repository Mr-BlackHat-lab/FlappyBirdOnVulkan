#pragma once

#include "../VulkanState.hpp"

namespace Semaphore {
    void createSyncObjects(VulkanState& state);

    void cleanupSyncObjects(VulkanState& state);
}