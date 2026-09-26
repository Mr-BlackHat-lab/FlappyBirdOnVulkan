#pragma once

#include "../VulkanState.hpp"

namespace  Application {

    //create glfw window
    void init(VulkanState& state, int width, int height, const char* title);

    //update cycle
    void update(VulkanState& state);

    // cleanup the glfw window
    void cleanup(const VulkanState& state);

    //should close
    bool shouldClose(const VulkanState& state);
};