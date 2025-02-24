cmake_minimum_required(VERSION 3.10)
project(FLUDER)

find_package(glfw3 REQUIRED)
find_package(Vulkan REQUIRED)

add_executable(FLUDER 
src/main.cpp 
src/Window.cpp 
src/Engine.cpp
src/Renderer.cpp
src/Vulkan/Instance.cpp
src/Vulkan/Device.cpp
)
target_link_libraries(FLUDER glfw Vulkan::Vulkan)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)