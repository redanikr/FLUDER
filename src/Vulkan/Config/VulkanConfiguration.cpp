#include "VulkanConfiguration.h"

const std::vector<const char *> FLUDER::Vulkan::VulkanConfiguration::instanceExtensions = {VK_KHR_SURFACE_EXTENSION_NAME, VK_EXT_DEBUG_UTILS_EXTENSION_NAME};
const std::vector<const char *> FLUDER::Vulkan::VulkanConfiguration::enabledLayers = {"VK_LAYER_KHRONOS_validation"};

VKAPI_ATTR VkBool32 VKAPI_CALL FLUDER::Vulkan::VulkanConfiguration::debugUtilsMessageCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT *pCallbackData, void *pUserData){
    std::cout << pCallbackData->pMessage << std::endl;
    return false;
}


FLUDER::Vulkan::VulkanConfiguration::VulkanConfiguration(bool validationEnabled) : m_validationEnabled(validationEnabled){
    createInstance();
}

FLUDER::Vulkan::VulkanConfiguration::~VulkanConfiguration(){
    auto func = (PFN_vkDestroyDebugUtilsMessengerEXT) vkGetInstanceProcAddr(m_instance, "vkDestroyDebugUtilsMessengerEXT");
    func(m_instance, m_debugUtilsMessengerEXT, nullptr);
    vkDestroyInstance(m_instance, nullptr);
}

void FLUDER::Vulkan::VulkanConfiguration::createInstance(){

    m_applicationInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	m_applicationInfo.pApplicationName = "FLUDER";
	m_applicationInfo.pEngineName = "FLUDER";
	m_applicationInfo.apiVersion = VK_API_VERSION_1_3;

    m_debugUtilsMessengerCreateInfoEXT.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
    m_debugUtilsMessengerCreateInfoEXT.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
    m_debugUtilsMessengerCreateInfoEXT.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
    m_debugUtilsMessengerCreateInfoEXT.pfnUserCallback = debugUtilsMessageCallback;

    m_instanceCreateInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    m_instanceCreateInfo.pApplicationInfo = &m_applicationInfo;
    m_instanceCreateInfo.enabledExtensionCount = static_cast<uint32_t>(instanceExtensions.size());
    m_instanceCreateInfo.ppEnabledExtensionNames = instanceExtensions.data();
    m_instanceCreateInfo.enabledLayerCount = static_cast<uint32_t>(enabledLayers.size());
    m_instanceCreateInfo.ppEnabledLayerNames = enabledLayers.data();
    m_instanceCreateInfo.pNext = &m_debugUtilsMessengerCreateInfoEXT;

    vkCreateInstance(&m_instanceCreateInfo,nullptr,&m_instance);
    auto func = (PFN_vkCreateDebugUtilsMessengerEXT) vkGetInstanceProcAddr(m_instance, "vkCreateDebugUtilsMessengerEXT");
    if (func != nullptr)
        func(m_instance, &m_debugUtilsMessengerCreateInfoEXT, nullptr, &m_debugUtilsMessengerEXT);
}

void FLUDER::Vulkan::VulkanConfiguration::choosePhysicalDevice(){
    uint32_t deviceCount = 0;
    std::vector<VkPhysicalDevice> devices(10);
    vkEnumeratePhysicalDevices(m_instance, &deviceCount, devices.data());
    m_physicalDevice = devices[0];
}

void FLUDER::Vulkan::VulkanConfiguration::createDevice(){
}
