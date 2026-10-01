#pragma once

#define GLFW_INCLUDE_VULKAN
#include <vector>
#include <GLFW/glfw3.h>
#include <algorithm>


struct VulkanState {

    GLFWwindow* window= nullptr;

    VkInstance instance = VK_NULL_HANDLE;
    VkSurfaceKHR surface = VK_NULL_HANDLE;
    VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
    VkDevice device = VK_NULL_HANDLE;
    VkQueue graphicsQueue = VK_NULL_HANDLE;
    VkQueue presentQueue = VK_NULL_HANDLE;

    // Added to determine swapchain sharing mode
    uint32_t graphicsQueueFamily = 0;
    uint32_t presentQueueFamily = 0;

    VkSwapchainKHR swapchain = VK_NULL_HANDLE;
    std::vector<VkImage> swapchainImages;
    VkFormat swapchainFormat;
    VkExtent2D swapchainExtent;

    // Added to interface with the swapchain images during rendering
    std::vector<VkImageView> swapchainImageViews;

    //Pipeline
    VkRenderPass renderPass = VK_NULL_HANDLE;

    //Buffer
    std::vector<VkFramebuffer> swapchainFramebuffers;


};