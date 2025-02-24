#include "Device.h"
#include "Instance.h"
#include <vector>
#include <vulkan/vulkan_core.h>
FLUDER::Vulkan::Device::Device(FLUDER::Vulkan::Instance instance){
    std::vector<VkPhysicalDevice> devices(2);
    unsigned int deviceCount = 2;
    vkEnumeratePhysicalDevices(instance.getVkInstance(), &deviceCount, devices.data());
    VkPhysicalDeviceFeatures features;
    VkPhysicalDeviceProperties properties;
    vkGetPhysicalDeviceFeatures(devices[0], &features);
    vkGetPhysicalDeviceProperties(devices[0], &properties);
    int b = 45;
}
VkDevice FLUDER::Vulkan::Device::getDevice(){

}
VkPhysicalDevice FLUDER::Vulkan::Device::getPhysicalDevice(){

}