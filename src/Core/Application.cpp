#include "Application.h"

#include  <stdexcept>
#include <cstdio>

namespace Application {
    void init(VulkanState& state, int width, int height, const char* title) {
        glfwInit();
        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

        state.window = glfwCreateWindow(width, height, title, nullptr, nullptr);
        printf("Window Created\n");

        if (!state.window) {
            throw std::runtime_error("Failed to create GLFW window\n");
        }
    }
    void update(VulkanState& state) {
        glfwPollEvents();
        // printf("Window Updated\n");
    }
    void cleanup(const VulkanState& state) {
        glfwDestroyWindow(state.window);
        printf("window Destroyed\n");
        glfwTerminate();
        printf("window cleaned\n");
    }
    bool shouldClose(const VulkanState& state) {
        return glfwWindowShouldClose(state.window);
    }
}
