#include "Buffer.h"
#include "VulkanContext.h"

#include <cstdio>
#include <stdexcept>

namespace Buffer {
    void createFramebuffers(VulkanState& state) {
        state.swapchainFramebuffers.resize(state.swapchainImageViews.size());

        for (size_t i = 0; i < state.swapchainImageViews.size(); i++) {
            VkImageView attachments[] = {
                state.swapchainImageViews[i]
            };
            VkFramebufferCreateInfo framebufferInfo{};
            framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;

            //link the framebuffer to render pass
            framebufferInfo.renderPass = state.renderPass;

            //bind the specific image view to attachment slot 0
            framebufferInfo.attachmentCount = 1;
            framebufferInfo.pAttachments = attachments;

            //set the dimension of the framebuffer to match the window
            framebufferInfo.width = state.swapchainExtent.width;
            framebufferInfo.height = state.swapchainExtent.height;
            framebufferInfo.layers = 1;

            if (vkCreateFramebuffer(state.device, &framebufferInfo, nullptr, &state.swapchainFramebuffers[i]) != VK_SUCCESS) {
                throw std::runtime_error("failed to create framebuffer!");
            }
        }
        printf("Framebuffer created\n");

    }
    void destroyFramebuffers(VulkanState& state) {
        for (auto& framebuffer : state.swapchainFramebuffers) {
            vkDestroyFramebuffer(state.device, framebuffer, nullptr);
        }
        printf("Buffer destroyed\n");
    }

    void createCommandPool(VulkanState& state) {
        QueueFamilyIndices queueFamilyIndices = VulkanContext::findQueueFamilies(state.physicalDevice, state.surface);

        VkCommandPoolCreateInfo poolInfo{};
        poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;

        // This flag is incredibly important! It allows us to reset and rerecord
        // the command buffer every single frame.
        poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;

        // Tie the pool to the graphics queue family
        poolInfo.queueFamilyIndex = queueFamilyIndices.graphicsFamily.value();

        if (vkCreateCommandPool(state.device, &poolInfo, nullptr, &state.commandPool) != VK_SUCCESS) {
            throw std::runtime_error("Failed to create command pool!");
        }

        printf("Command Pool created successfully.\n");
    }
    void createCommandBuffer(VulkanState& state) {
        VkCommandBufferAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        allocInfo.commandPool = state.commandPool;

        // Primary command buffers can be submitted directly to a queue.
        // Secondary command buffers can only be called from inside primary command buffers.
        allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        allocInfo.commandBufferCount = 1;

        if (vkAllocateCommandBuffers(state.device, &allocInfo, &state.commandBuffer) != VK_SUCCESS) {
            throw std::runtime_error("Failed to allocate command buffers!");
        }

        printf("Command Buffer allocated successfully.\n");
    }

    void destroyCommandPool(VulkanState &state) {
        if (state.commandPool != VK_NULL_HANDLE) {
            vkDestroyCommandPool(state.device, state.commandPool, nullptr);
            state.commandPool = VK_NULL_HANDLE;
            printf("Command Buffer destroyed successfully.\n");
            printf("Command Pool destroyed successfully.\n");
        }
    }

    void init(VulkanState& state) {
        createFramebuffers(state);
        createCommandPool(state);
        createCommandBuffer(state);
    }
    void cleanup(VulkanState& state) {
        destroyFramebuffers(state);
        destroyCommandPool(state);
    }
}
