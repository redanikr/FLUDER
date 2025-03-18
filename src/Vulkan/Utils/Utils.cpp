#include "Utils.h"
#include <GLFW/glfw3.h>
#include <fstream>
std::vector<VkPhysicalDevice> fldr::vk::utils::availablePhysicalDevices(VkInstance instance){
    uint32_t deviceCount = 0;
    vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);
    std::vector<VkPhysicalDevice> devices(deviceCount);
    vkEnumeratePhysicalDevices(instance, &deviceCount, devices.data());
    return devices;
}
std::vector<const char*> fldr::vk::utils::vkInstanceExtensions(bool validationEnabled) {
    std::vector<const char*> extensions = glfwExtensions();
    if (validationEnabled)
        extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
    return extensions;
}

std::vector<const char*> fldr::vk::utils::layers() {
    return { "VK_LAYER_KHRONOS_validation" };
}

std::vector<const char*> fldr::vk::utils::vkDeviceExtensions() {
    return { VK_KHR_SWAPCHAIN_EXTENSION_NAME };
}

std::vector<const char*> fldr::vk::utils::glfwExtensions(){
    uint32_t glfwExtensionCount = 0;
    const char** glfwExtensions;
    glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
    return std::vector<const char*>(glfwExtensions, glfwExtensions + glfwExtensionCount);
}

std::vector<VkQueueFamilyProperties> fldr::vk::utils::physicalDeviceQueueFamilyProperties(VkPhysicalDevice physicalDevice) {
    uint32_t queueCount = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueCount, nullptr);
    std::vector<VkQueueFamilyProperties> properties(queueCount);
    vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueCount, properties.data());
    return properties;
}

std::vector<uint32_t> fldr::vk::utils::graphicsQueueIndeces(VkPhysicalDevice physicalDevice) {
    std::vector<VkQueueFamilyProperties> properties = physicalDeviceQueueFamilyProperties(physicalDevice);
    std::vector<uint32_t> indeces;
    for (int i = 0; i < properties.size(); i++)
        if (properties[i].queueFlags & VK_QUEUE_GRAPHICS_BIT)
            indeces.push_back(i);
    return indeces;
}

VkQueue fldr::vk::utils::queueHandle(VkDevice device, uint32_t queueFamilyIndex) {
    VkQueue queueHandle = VK_NULL_HANDLE;
    vkGetDeviceQueue(device, queueFamilyIndex, 0, &queueHandle);
    return queueHandle;
}

std::vector<uint32_t> fldr::vk::utils::presentQueueIndeces(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface) {
    std::vector<VkQueueFamilyProperties> properties = physicalDeviceQueueFamilyProperties(physicalDevice);
    std::vector<uint32_t> indeces;
    for (int i = 0; i < properties.size(); i++) {
        VkBool32 canPresent = false;
        vkGetPhysicalDeviceSurfaceSupportKHR(physicalDevice, i, surface, &canPresent);
        if (properties[i].queueFlags)
            indeces.push_back(i);
    }
    return indeces;
}

VkDebugUtilsMessengerEXT fldr::vk::utils::createDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT & debugUtilsMessengerCreateInfoEXT) {
    auto func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
    VkDebugUtilsMessengerEXT debugUtilsMessengerEXT = VK_NULL_HANDLE;
    if (func != nullptr)
        func(instance, &debugUtilsMessengerCreateInfoEXT, nullptr, &debugUtilsMessengerEXT);
    return debugUtilsMessengerEXT;
}

void fldr::vk::utils::destroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugUtilsMessengerEXT) {
    auto func = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
    func(instance, debugUtilsMessengerEXT, nullptr);
}

VkSurfaceCapabilitiesKHR fldr::vk::utils::surfaceCapabilities(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface) {
    VkSurfaceCapabilitiesKHR surfaceCapabilitiesKHR;
    vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physicalDevice, surface, &surfaceCapabilitiesKHR);
    return surfaceCapabilitiesKHR;
}

std::vector<VkSurfaceFormatKHR> fldr::vk::utils::surfaceFormats(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface) {
    uint32_t physicalDeviceSurfaceFormatCount = 0;
    vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, surface, &physicalDeviceSurfaceFormatCount, nullptr);
    std::vector<VkSurfaceFormatKHR> surfaceFormats(physicalDeviceSurfaceFormatCount);
    vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, surface, &physicalDeviceSurfaceFormatCount, surfaceFormats.data());
    return surfaceFormats;
}

std::vector<VkImage> fldr::vk::utils::swapchainImages(VkDevice device, VkSwapchainKHR swapchain) {
    uint32_t swapchainImageCount = 0;
    vkGetSwapchainImagesKHR(device, swapchain, &swapchainImageCount, nullptr);
    std::vector<VkImage> swapchainImages(swapchainImageCount);
    vkGetSwapchainImagesKHR(device, swapchain, &swapchainImageCount, swapchainImages.data());
    return swapchainImages;
}

std::vector<char> fldr::vk::utils::readShaderFile(std::string filePath) {
    std::vector<char> shaderCode;
    std::ifstream file1 = std::ifstream(filePath, std::ios_base::binary);
    while (1) {
        int byte = file1.get();
        if (byte == -1)
            break;
        shaderCode.push_back(byte);
    }
    return shaderCode;
}
