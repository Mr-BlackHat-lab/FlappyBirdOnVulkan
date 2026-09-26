#include "VulkanState.hpp"
#include "Core/Application.h"
#include "Vulkan/VulkanContext.h"


int main() {
    VulkanState state;

    Application::init(state,600,800,"FlappyBird");
    VulkanContext::init(state);

    while (!Application::shouldClose(state)) {
        Application::update(state);
    }

    VulkanContext::cleanup(state);
    Application::cleanup(state);
    return 0;
}
