#include <vector>
#include "Utils/Utils.h"
#include "Device.h"
fldr::vk::Device::Device(const Instance & instance, bool validationEnabled) : m_physicalDevice(PhysicalDevice(instance)){

    m_physicalDevice = PhysicalDevice(instance);

    float queuePriority = 1.0f;
    VkDeviceQueueCreateInfo deviceQueueCreateInfo = {};
    deviceQueueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    deviceQueueCreateInfo.queueFamilyIndex = m_physicalDevice.graphicsQueueFamilyIndex();
    deviceQueueCreateInfo.queueCount = 1;
    deviceQueueCreateInfo.pQueuePriorities = &queuePriority;

    VkPhysicalDeviceSynchronization2Features physicalDeviceSynchronization2Features = {};
    physicalDeviceSynchronization2Features.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SYNCHRONIZATION_2_FEATURES;
    physicalDeviceSynchronization2Features.synchronization2 = VK_TRUE;

    VkPhysicalDeviceDynamicRenderingFeatures physicalDeviceDynamicRenderingFeatures = {};
    physicalDeviceDynamicRenderingFeatures.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_DYNAMIC_RENDERING_FEATURES_KHR;
    physicalDeviceDynamicRenderingFeatures.dynamicRendering = VK_TRUE;
    physicalDeviceDynamicRenderingFeatures.pNext = &physicalDeviceSynchronization2Features;

    VkPhysicalDeviceFeatures2 features = {};
    features.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FEATURES_2;
    vkGetPhysicalDeviceFeatures2(m_physicalDevice.get(), &features);
    features.pNext = &physicalDeviceDynamicRenderingFeatures;

    std::vector<const char*> layers = fldr::vk::utils::layers();
    std::vector<const char*> extensions = fldr::vk::utils::vkDeviceExtensions();
    VkDeviceCreateInfo deviceCreateInfo = {};
    deviceCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    deviceCreateInfo.pNext = &features;
    deviceCreateInfo.pQueueCreateInfos = &deviceQueueCreateInfo;
    deviceCreateInfo.queueCreateInfoCount = 1;
    deviceCreateInfo.ppEnabledExtensionNames = extensions.data();
    deviceCreateInfo.enabledExtensionCount = extensions.size();
    if (validationEnabled) {
        deviceCreateInfo.ppEnabledLayerNames = layers.data();
        deviceCreateInfo.enabledLayerCount = layers.size();
    }
    vkCreateDevice(m_physicalDevice.get(), &deviceCreateInfo, nullptr, &m_device);

    vkGetDeviceQueue(m_device, m_physicalDevice.graphicsQueueFamilyIndex(), 0, &m_graphicsQueueHandle);
}

fldr::vk::Device::~Device() {
    vkDestroyDevice(m_device, nullptr);
}

VkDevice fldr::vk::Device::get() const {
    return m_device;
}

const fldr::vk::PhysicalDevice& fldr::vk::Device::physicalDevice() const {
    return m_physicalDevice;
}

VkQueue fldr::vk::Device::graphicsQueueHandle() const {
    return m_graphicsQueueHandle;
}
