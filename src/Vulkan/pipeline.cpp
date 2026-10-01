#include "pipeline.h"

#include <cstdio>
#include <stdexcept>

namespace Pipeline {
    void createRenderPass(VulkanState& state) {
        // 1. Define the Color Attachment (The Swapchain Image)
        VkAttachmentDescription colorAttachment{};
        colorAttachment.format = state.swapchainFormat;          // Match the swapchain format
        colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;         // No multisampling (anti-aliasing) yet

        // What to do with the data before and after rendering
        colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;    // Clear the screen to a solid color before drawing
        colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;  // Store the rendered result in memory so we can see it

        // We don't care about stencil data for Flappy Bird
        colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
        colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;

        // Image layouts (Vulkan needs to transition images between optimal memory layouts)
        colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;      // We don't care what format the image is in before the pass starts
        colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;  // After the pass, the image must be ready to present to the screen

        // 2. Subpasses and Attachment References
        // A render pass can consist of multiple subpasses, but we only need one.
        VkAttachmentReference colorAttachmentRef{};
        colorAttachmentRef.attachment = 0; // Index of the attachment in the description array
        colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL; // Optimal layout for drawing

        VkSubpassDescription subpass{};
        subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
        subpass.colorAttachmentCount = 1;
        subpass.pColorAttachments = &colorAttachmentRef;

        // 3. Subpass Dependencies (Crucial for synchronization)
        // This tells Vulkan not to start the layout transition until the image is actually acquired from the swapchain
        VkSubpassDependency dependency{};
        dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
        dependency.dstSubpass = 0; // Our single subpass
        dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
        dependency.srcAccessMask = 0;
        dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
        dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

        // 4. Create the Render Pass Object
        VkRenderPassCreateInfo renderPassInfo{};
        renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
        renderPassInfo.attachmentCount = 1;
        renderPassInfo.pAttachments = &colorAttachment;
        renderPassInfo.subpassCount = 1;
        renderPassInfo.pSubpasses = &subpass;
        renderPassInfo.dependencyCount = 1;
        renderPassInfo.pDependencies = &dependency;

        if (vkCreateRenderPass(state.device, &renderPassInfo, nullptr, &state.renderPass) != VK_SUCCESS) {
            throw std::runtime_error("Failed to create render pass!");
        }

        printf("Render Pass created successfully.\n");
    }

    void cleanup(VulkanState& state) {
        if (state.renderPass != VK_NULL_HANDLE) {
            vkDestroyRenderPass(state.device, state.renderPass, nullptr);
            state.renderPass = VK_NULL_HANDLE;
            printf("Pipeline destroyed successfully\n");
        }
    }
}
