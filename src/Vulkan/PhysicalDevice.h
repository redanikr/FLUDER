#pragma once
#include <vulkan/vulkan.h>
#include "Instance.h"

namespace fldr::vk {
	class PhysicalDevice {
	public:
		PhysicalDevice(const Instance & instance);
		VkPhysicalDevice get()const;
		uint32_t graphicsQueueFamilyIndex()const;
	private:
		VkPhysicalDevice m_physicalDevice;
		uint32_t m_graphicsQueueFamilyIndex;
	};
}