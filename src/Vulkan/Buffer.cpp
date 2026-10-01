#include "Buffer.h"

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
}
