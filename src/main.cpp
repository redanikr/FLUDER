#include "Window.h"
#include "Vulkan/Instance.h"
#include "Vulkan/PhysicalDevice.h"
#include "Vulkan/Device.h"
#include "Vulkan/Surface.h"
#include "Vulkan/Swapchain.h"
#include "Vulkan/Shader.h"
#include "Vulkan/Pipeline.h"
int main(){
    fldr::Window window;
    fldr::vk::Instance instance(true);
    fldr::vk::Device device(instance,true);
    fldr::vk::Surface surface(instance, window);
    fldr::vk::Shader vertexShader(device, "shader.vert");
    fldr::vk::Shader fragmentShader(device, "shader.frag");
    fldr::vk::Pipeline pipeline(device, surface, vertexShader, fragmentShader);
    return 0;

}
