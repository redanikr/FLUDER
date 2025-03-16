#pragma once


#include <vector>
#include <vulkan/vulkan.h>
#include <string>
namespace fldr::vk::utils{
    std::vector<VkPhysicalDevice> availablePhysicalDevices(VkInstance instance);
    std::vector<const char*> vkInstanceExtensions(bool validationEnabled);
    std::vector<const char*> layers();
    std::vector<const char*> vkDeviceExtensions();
    std::vector<const char*> glfwExtensions();
    std::vector<VkQueueFamilyProperties> physicalDeviceQueueFamilyProperties(VkPhysicalDevice physicalDevice);
    std::vector<uint32_t> graphicsQueueIndeces(VkPhysicalDevice physicalDevice);
    VkQueue queueHandle(VkDevice device, uint32_t graphicsQueueFamilyIndex);
    std::vector<uint32_t> presentQueueIndeces(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface);
    VkDebugUtilsMessengerEXT createDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT & debugUtilsMessengerCreateInfoEXT);
    void destroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugUtilsMessengerEXT);
    VkSurfaceCapabilitiesKHR surfaceCapabilities(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface);
    std::vector<VkSurfaceFormatKHR> surfaceFormats(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface);
    std::vector<VkImage> swapchainImages(VkDevice device, VkSwapchainKHR swapchain);
    std::vector<char> readShaderFile(std::string filePath);
}