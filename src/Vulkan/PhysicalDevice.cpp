#include "PhysicalDevice.h"
#include "Utils/Utils.h"

fldr::vk::PhysicalDevice::PhysicalDevice(const Instance& instance) : 
	m_physicalDevice(fldr::vk::utils::availablePhysicalDevices(instance.get())[0]),
	m_graphicsQueueFamilyIndex(fldr::vk::utils::graphicsQueueIndeces(m_physicalDevice)[0]){
}

VkPhysicalDevice fldr::vk::PhysicalDevice::get() const {
	return m_physicalDevice;
}

uint32_t fldr::vk::PhysicalDevice::graphicsQueueFamilyIndex() const {
	return m_graphicsQueueFamilyIndex;
}
