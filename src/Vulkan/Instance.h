#pragma once

#include <vulkan/vulkan_core.h>
#include <vector>
namespace FLUDER::Vulkan {

    static std::vector<const char *> instanceExtensions;
    static std::vector<const char *> enabledLayers;
    VKAPI_ATTR VkBool32 VKAPI_CALL debugUtilsMessageCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData);
    
    class Instance{
        public:
            static const std::vector<const char *> instanceExtensions;
            static const std::vector<const char *> enabledLayers;
            Instance(bool validationEnabled);
            ~Instance();
            VkInstance getVkInstance();
        private:
            bool m_validationEnabled;
            VkInstance m_instance;
            VkDebugUtilsMessengerEXT m_debugUtilsMessengerEXT;
            VkApplicationInfo m_applicationInfo = {};
            VkInstanceCreateInfo m_instanceCreateInfo = {};
            VkDebugUtilsMessengerCreateInfoEXT m_debugUtilsMessengerCreateInfoEXT = {};
            void fillDebugUtilsMessengerCreateInfoEXT();
            void fillInstanceCreateInfo();
            void fillApplicationInfo();
            void createInstance();
            void createDebugUtilsMessengerEXT();
    };
}