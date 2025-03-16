#include "ImageView.h"
#include "Config/Config.h"
#include "Device.h"

fldr::vk::ImageView::ImageView(const Device & device, VkImage image, VkFormat surfaceFormat) : m_device(device){
	VkImageViewCreateInfo imageCreateInfo = fldr::vk::config::imageViewCreateInfo(image, surfaceFormat);
	vkCreateImageView(device.get(), &imageCreateInfo, nullptr, &m_imageView);
}

VkImageView fldr::vk::ImageView::get() const {
	return m_imageView;
}

VkImage fldr::vk::ImageView::image() const {
	return m_image;
}

fldr::vk::ImageView::~ImageView() {
	vkDestroyImageView(m_device.get(), m_imageView, nullptr);
}
