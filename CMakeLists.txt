cmake_minimum_required(VERSION 3.10)
project(FLUDER)

find_package(Vulkan REQUIRED)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_COMPILER)
add_executable(FLUDER 

src/main.cpp
src/Vulkan/Config/Config.cpp
src/Vulkan/Utils/Utils.cpp
src/Window.cpp 
src/Engine.cpp
src/Vulkan/Renderer.cpp
src/Vulkan/Instance.cpp
src/Vulkan/PhysicalDevice
src/Vulkan/Device.cpp
src/Vulkan/Surface.cpp
src/Vulkan/Swapchain.cpp
src/Vulkan/Shader.cpp
src/Vulkan/Surface.cpp
src/Vulkan/Pipeline.cpp
src/Vulkan/ImageView.cpp
src/Vulkan/CommandBuffer.cpp
src/Vulkan/Buffer.h
src/Vulkan/VertexBuffer.cpp
src/Vulkan/IndexBuffer.cpp

)

configure_file(./shaders/shader.vert shader.vert COPYONLY)
configure_file(./shaders/shader.frag shader.frag COPYONLY)
target_include_directories(FLUDER PUBLIC GLFW/)
target_link_directories(FLUDER PUBLIC GLFW/)
target_link_libraries(FLUDER glfw3 Vulkan::Vulkan)