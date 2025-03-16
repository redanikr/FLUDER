#pragma once

#include <vulkan/vulkan.h>
#include "Instance.h"
#include "PhysicalDevice.h"
namespace fldr::vk {
	class Device {
	public:
		Device(const Instance & instance, bool validationEnabled);
		~Device();
		VkDevice get()const;
		const PhysicalDevice & physicalDevice()const;
		VkQueue graphicsQueueHandle()const;

	private:
		VkDevice m_device;
		PhysicalDevice m_physicalDevice;
		VkQueue m_graphicsQueueHandle;
	};
}