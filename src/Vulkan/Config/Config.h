#pragma once

#include <vulkan/vulkan.h>
#include <vector>
namespace fldr::vk::config {
    VKAPI_ATTR VkBool32 VKAPI_CALL debugUtilsMessageCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData);
    VkApplicationInfo applicationInfo();
    VkDebugUtilsMessengerCreateInfoEXT debugUtilsMessengerCreateInfoEXT();
    VkInstanceCreateInfo instanceCreateInfo(const VkApplicationInfo& applicationInfo, const VkDebugUtilsMessengerCreateInfoEXT& debugUtilsMessengerCreateInfoEXT, bool validationEnabled, const std::vector<const char*>& extensions, const std::vector<const char*>& layers);
    VkSwapchainCreateInfoKHR swapchainCreateInfo(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface);
    VkImageViewCreateInfo imageViewCreateInfo(VkImage image, VkFormat surfaceFormat);
}