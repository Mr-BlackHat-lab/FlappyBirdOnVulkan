#include "Semaphore.h"

#include <cstdio>
#include <stdexcept>

namespace Semaphore {
    void createSyncObjects(VulkanState& state) {
        VkSemaphoreCreateInfo semaphoreInfo{};
        semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

        VkFenceCreateInfo fenceInfo{};
        fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;

        // CRITICAL: Create the fence in the "Signaled" state.
        // In our render loop, the CPU will wait for this fence before drawing.
        // If we don't start it as signaled, the CPU will wait forever on the very first frame!
        fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

        if (vkCreateSemaphore(state.device, &semaphoreInfo, nullptr, &state.imageAvailableSemaphore) != VK_SUCCESS ||
            vkCreateSemaphore(state.device, &semaphoreInfo, nullptr, &state.renderFinishedSemaphore) != VK_SUCCESS ||
            vkCreateFence(state.device, &fenceInfo, nullptr, &state.inFlightFence) != VK_SUCCESS) {
            throw std::runtime_error("Failed to create synchronization objects!");
            }

        printf("Semaphores and Fences created successfully.\n");
    }

    void cleanupSyncObjects(VulkanState &state) {
        vkDestroySemaphore(state.device, state.renderFinishedSemaphore, nullptr);
        vkDestroySemaphore(state.device, state.imageAvailableSemaphore, nullptr);
        vkDestroyFence(state.device, state.inFlightFence, nullptr);
        printf("Semaphores and Fences destroyed successfully.\n");
    }
}
