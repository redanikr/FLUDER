#pragma once

#include <vulkan/vulkan.h>
#include "Surface.h"
#include "Device.h"
#include <vector>
#include <memory>
#include "ImageView.h"
namespace fldr::vk {
	class Swapchain {
	public:
		Swapchain(const Device & device, const Surface & surface);
		~Swapchain();
		VkSwapchainKHR get()const;

	private:
		const Device& m_device;
		VkSwapchainKHR m_swapchain;
		std::vector<VkImage> m_swapchainImages;
		std::vector<std::unique_ptr<ImageView>> m_imageViews;
		uint32_t m_presentQueueFamilyIndex;
		VkQueue m_presentQueueHandle;
	};
}