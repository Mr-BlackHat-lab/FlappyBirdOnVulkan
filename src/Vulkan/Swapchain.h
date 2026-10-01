#pragma once

#include "../VulkanState.hpp"
#include <vector>

struct SwapChainSupportDetails {
    VkSurfaceCapabilitiesKHR capabilities;
    std::vector<VkSurfaceFormatKHR> formats;
    std::vector<VkPresentModeKHR> presentModes;
};

namespace Swapchain {
    // Expose this so VulkanContext can use it during physical device selection
    SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device, VkSurfaceKHR surface);

    // Call this after the logical device is created
    void createSwapchain(VulkanState& state);
    void createImageViews(VulkanState& state);
}