#include "Swapchain.h"

#include <algorithm>
#include <limits>
#include <set>
#include <stdexcept>
#include <string>

namespace Swapchain {
    SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device, VkSurfaceKHR surface) {
        SwapChainSupportDetails details;

        // Basic surface capabilities (min/max number of images in swapchain, min/max width and height)
        vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, surface, &details.capabilities);

        // Supported surface formats (pixel format, color space)
        uint32_t formatCount;
        vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, nullptr);
        if (formatCount!=0) {
            details.formats.resize(formatCount);
            vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, details.formats.data());
        }
        // Supported presentation modes
        uint32_t presentModeCount;
        vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModeCount, nullptr);
        if (presentModeCount!=0) {
            details.presentModes.resize(presentModeCount);
            vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModeCount, details.presentModes.data());
        }

        return details;
    }
    void createSwapchain(VulkanState& state) {
        SwapChainSupportDetails swapChainSupport = querySwapChainSupport(state.physicalDevice, state.surface);

        //Format pick
        VkSurfaceFormatKHR surfaceFormat = swapChainSupport.formats[0];
        for (const auto& availableFormat : swapChainSupport.formats) {
            if (availableFormat.format == VK_FORMAT_B8G8R8A8_SRGB && availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
                surfaceFormat = availableFormat;
                break;
            }
        }

        // PresentMode
        VkPresentModeKHR presentMode = VK_PRESENT_MODE_FIFO_KHR;
        for (const auto& availablePresentMode : swapChainSupport.presentModes) {
            if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR) {
                presentMode = availablePresentMode;
                break;
            }
        }

        //Swap Extent
        VkExtent2D swapChainExtent;
        if (swapChainSupport.capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max()) {
            swapChainExtent = swapChainSupport.capabilities.currentExtent;
        }
        else {
            int width, height;
            glfwGetFramebufferSize(state.window, &width, &height);

            swapChainExtent = {
                static_cast<uint32_t>(width),
                static_cast<uint32_t>(height)
            };
            swapChainExtent.width = std::clamp(swapChainExtent.width,swapChainSupport.capabilities.minImageExtent.width, swapChainSupport.capabilities.maxImageExtent.width);
            swapChainExtent.height = std::clamp(swapChainExtent.height,swapChainSupport.capabilities.minImageExtent.height, swapChainSupport.capabilities.maxImageExtent.height);
        }

        //Image Count
        uint32_t swapchainImageCount = swapChainSupport.capabilities.minImageCount + 1;
        if (swapChainSupport.capabilities.maxImageCount > 0 && swapchainImageCount > swapChainSupport.capabilities.maxImageCount) {
            swapchainImageCount = swapChainSupport.capabilities.maxImageCount;
        }


        //Create Info
        VkSwapchainCreateInfoKHR createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
        createInfo.surface = state.surface;
        createInfo.minImageCount = swapchainImageCount;
        createInfo.imageFormat = surfaceFormat.format;
        createInfo.imageColorSpace = surfaceFormat.colorSpace;
        createInfo.imageExtent = swapChainExtent;
        createInfo.imageArrayLayers = 1;
        createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

        uint32_t queueFamilyIndices[] = {state.graphicsQueueFamily, state.presentQueueFamily};

        if (state.graphicsQueueFamily != state.presentQueueFamily) {
            // Queues are different: Image must be shared concurrently
            createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
            createInfo.queueFamilyIndexCount = 2;
            createInfo.pQueueFamilyIndices = queueFamilyIndices;
        } else {
            // Queues are the same (most common): Exclusive mode offers better performance
            createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
            createInfo.queueFamilyIndexCount = 0; // Optional
            createInfo.pQueueFamilyIndices = nullptr; // Optional
        }

        createInfo.preTransform = swapChainSupport.capabilities.currentTransform;
        createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
        createInfo.presentMode = presentMode;
        createInfo.clipped = VK_TRUE;
        createInfo.oldSwapchain = VK_NULL_HANDLE;

        if (vkCreateSwapchainKHR(state.device, &createInfo, nullptr, &state.swapchain) != VK_SUCCESS) {
            throw std::runtime_error("Failed to create swapchain!");
        }


        vkGetSwapchainImagesKHR(state.device, state.swapchain, &swapchainImageCount, nullptr);
        state.swapchainImages.resize(swapchainImageCount);
        vkGetSwapchainImagesKHR(state.device, state.swapchain, &swapchainImageCount, state.swapchainImages.data());

        state.swapchainFormat = surfaceFormat.format;
        state.swapchainExtent = swapChainExtent;

        printf("Swapchain created successfully.\n");
    }

    void createImageViews(VulkanState &state) {
        state.swapchainImages.resize((state.swapchainImageViews.size()));
        for (size_t i = 0; i < state.swapchainImages.size(); i++) {
            VkImageViewCreateInfo imageInfo{};
            imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
            imageInfo.image = state.swapchainImages[i];


            imageInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
            imageInfo.format = state.swapchainFormat;

            imageInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
            imageInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
            imageInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
            imageInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;

            imageInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
            imageInfo.subresourceRange.baseMipLevel = 0;
            imageInfo.subresourceRange.levelCount = 1;
            imageInfo.subresourceRange.baseArrayLayer = 0;
            imageInfo.subresourceRange.layerCount = 1;

            if (vkCreateImageView(state.device, &imageInfo, nullptr, &state.swapchainImageViews[i]) != VK_SUCCESS) {
                throw std::runtime_error("Failed to create image view!");
            }
        }
        printf("Image view created successfully.\n");
    }
}
