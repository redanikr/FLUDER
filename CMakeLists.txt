cmake_minimum_required(VERSION 3.10)
project(FLUDER)

find_package(Vulkan REQUIRED)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_COMPILER)
add_executable(FLUDER 

src/main.cpp 
src/Window.cpp 
src/Engine.cpp
src/Renderer.cpp
src/Vulkan/Instance.cpp
src/Vulkan/Device.cpp
src/Vulkan/Config/VulkanConfiguration.cpp
)
target_include_directories(FLUDER PUBLIC GLFW/)
target_link_directories(FLUDER PUBLIC GLFW/)
target_link_libraries(FLUDER glfw3 Vulkan::Vulkan)