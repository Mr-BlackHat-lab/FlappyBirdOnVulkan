#include "VulkanState.hpp"
#include "Core/Application.h"
#include "Vulkan/VulkanContext.h"
#include "Vulkan/pipeline.h"K


int main() {
    VulkanState state;

    Application::init(state,600,800,"FlappyBird");
    VulkanContext::init(state);
    Pipeline::createRenderPass(state);

    while (!Application::shouldClose(state)) {
        Application::update(state);
    }

    Pipeline::cleanup(state);
    VulkanContext::cleanup(state);
    Application::cleanup(state);
    return 0;
}
