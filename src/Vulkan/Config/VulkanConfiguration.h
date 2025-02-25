#pragma once

#include <vulkan/vulkan.h>
#include <vector>
#include <iostream>
namespace FLUDER::Vulkan {
    class VulkanConfiguration{
        public:   
        VulkanConfiguration(bool validationEnabled);
        ~VulkanConfiguration();
        static VKAPI_ATTR VkBool32 VKAPI_CALL debugUtilsMessageCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,VkDebugUtilsMessageTypeFlagsEXT messageType,const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,void* pUserData);
        
        private:
        void createInstance();
        void choosePhysicalDevice();
        void createDevice();
        
        bool m_validationEnabled;
        VkInstance m_instance;
        VkDebugUtilsMessengerEXT m_debugUtilsMessengerEXT;
        VkPhysicalDevice m_physicalDevice;
        
        static const std::vector<const char *> instanceExtensions;
        static const std::vector<const char *> enabledLayers;
        VkApplicationInfo m_applicationInfo = {};
        VkInstanceCreateInfo m_instanceCreateInfo = {};
        VkDebugUtilsMessengerCreateInfoEXT m_debugUtilsMessengerCreateInfoEXT = {};
    };
}