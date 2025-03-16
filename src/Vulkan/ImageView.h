#pragma once

#include <vulkan/vulkan.h>
#include "Device.h"
namespace fldr::vk {
	class ImageView {
	public:
		ImageView(const Device& device, VkImage image, VkFormat surfaceFormat);
		VkImageView get()const;
		VkImage image()const;
		~ImageView();
	private:
		const Device& m_device;
		VkImageView m_imageView;
		VkImage m_image;
	};
}