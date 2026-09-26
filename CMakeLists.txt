cmake_minimum_required(VERSION 4.3)
project(FlappyBird)


find_package(glfw3 CONFIG REQUIRED)
find_package(Vulkan REQUIRED COMPONENTS volk shaderc_combined)

set(CMAKE_CXX_STANDARD 20)

add_executable(FlappyBird src/main.cpp)

target_link_libraries(FlappyBird PRIVATE
        glfw
        Vulkan::Vulkan
        Vulkan::volk
        Vulkan::shaderc_combined
)