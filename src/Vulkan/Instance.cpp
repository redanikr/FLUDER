#include "Instance.h"
#include <vulkan/vulkan_core.h>
#include <iostream>

const std::vector<const char *> FLUDER::Vulkan::Instance::instanceExtensions = {VK_KHR_SURFACE_EXTENSION_NAME, VK_EXT_DEBUG_UTILS_EXTENSION_NAME};

const std::vector<const char *> FLUDER::Vulkan::Instance::enabledLayers = {"VK_LAYER_KHRONOS_validation"};

VKAPI_ATTR VkBool32 VKAPI_CALL FLUDER::Vulkan::debugUtilsMessageCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData){
    std::cout << pCallbackData->pMessage << std::endl;
    return VK_FALSE;
}
FLUDER::Vulkan::Instance::Instance(bool validationEnabled) : m_validationEnabled(validationEnabled){
    fillApplicationInfo();
    fillDebugUtilsMessengerCreateInfoEXT();
    fillInstanceCreateInfo();
    createInstance();
    createDebugUtilsMessengerEXT();
}
FLUDER::Vulkan::Instance::~Instance(){
    auto func = (PFN_vkDestroyDebugUtilsMessengerEXT) vkGetInstanceProcAddr(m_instance, "vkDestroyDebugUtilsMessengerEXT");
    func(m_instance, m_debugUtilsMessengerEXT, nullptr);
    vkDestroyInstance(m_instance, nullptr);
}
VkInstance FLUDER::Vulkan::Instance::getVkInstance(){
    return m_instance;
}
void FLUDER::Vulkan::Instance::fillDebugUtilsMessengerCreateInfoEXT(){
    m_debugUtilsMessengerCreateInfoEXT.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
    m_debugUtilsMessengerCreateInfoEXT.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
    m_debugUtilsMessengerCreateInfoEXT.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
    m_debugUtilsMessengerCreateInfoEXT.pfnUserCallback = debugUtilsMessageCallback;
}
void FLUDER::Vulkan::Instance::fillInstanceCreateInfo(){
    m_instanceCreateInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    m_instanceCreateInfo.pApplicationInfo = &m_applicationInfo;
    m_instanceCreateInfo.enabledExtensionCount = instanceExtensions.size();
    m_instanceCreateInfo.ppEnabledExtensionNames = instanceExtensions.data();
    m_instanceCreateInfo.enabledLayerCount = enabledLayers.size();
    m_instanceCreateInfo.ppEnabledLayerNames = enabledLayers.data();
    m_instanceCreateInfo.pNext = &m_debugUtilsMessengerCreateInfoEXT;
}
void FLUDER::Vulkan::Instance::fillApplicationInfo(){
    m_applicationInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	m_applicationInfo.pApplicationName = "FLUDER";
	m_applicationInfo.pEngineName = "FLUDER";
	m_applicationInfo.apiVersion = VK_API_VERSION_1_4;
}
void FLUDER::Vulkan::Instance::createInstance(){
    vkCreateInstance(&m_instanceCreateInfo,nullptr,&m_instance);
}
void FLUDER::Vulkan::Instance::createDebugUtilsMessengerEXT(){
    auto func = (PFN_vkCreateDebugUtilsMessengerEXT) vkGetInstanceProcAddr(m_instance, "vkCreateDebugUtilsMessengerEXT");
    if (func != nullptr)
        func(m_instance, &m_debugUtilsMessengerCreateInfoEXT, nullptr, &m_debugUtilsMessengerEXT);
}