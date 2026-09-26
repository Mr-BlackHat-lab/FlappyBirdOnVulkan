#include "VulkanState.hpp"
#include "Core/Application.h"


int main() {
    VulkanState state;

    Application::init(state,600,800,"FlappyBird");


    while (!Application::shouldClose(state)) {
        Application::update(state);
    }


    Application::cleanup(state);
    return 0;
}
