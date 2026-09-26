#include "Application.h"

#include <stdexcept>
#include <cstdio>

namespace Application {
    void init(VulkanState& state, int width, int height, const char* title) {
        // 1. Check if GLFW initializes successfully
        if (!glfwInit()) {
            throw std::runtime_error("Failed to initialize GLFW\n");
        }

        // Tell GLFW not to create an OpenGL context
        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

        state.window = glfwCreateWindow(width, height, title, nullptr, nullptr);

        // 2. Handle failure before printing success
        if (!state.window) {
            glfwTerminate(); // 3. Clean up GLFW if window creation fails
            throw std::runtime_error("Failed to create GLFW window\n");
        }

        printf("Window Created\n");
    }

    void update(VulkanState& state) {
        glfwPollEvents();
    }

    void cleanup(const VulkanState& state) {
        glfwDestroyWindow(state.window);
        printf("Window Destroyed\n");

        glfwTerminate();
        printf("GLFW Cleaned\n");
    }

    bool shouldClose(const VulkanState& state) {
        return glfwWindowShouldClose(state.window);
    }
}