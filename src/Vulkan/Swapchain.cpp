#include "Swapchain.h"
#include "Utils/Utils.h"
#include "Config/Config.h"
#include "ImageView.h"
fldr::vk::Swapchain::Swapchain(const Device& device, const Surface& surface) : m_device(device){
    VkSwapchainCreateInfoKHR swapchainCreateInfo = fldr::vk::config::swapchainCreateInfo(device.physicalDevice().get(), surface.get());
    vkCreateSwapchainKHR(device.get(), &swapchainCreateInfo, nullptr, &m_swapchain);
    m_swapchainImages = fldr::vk::utils::swapchainImages(device.get(), m_swapchain);
    for (size_t i = 0; i < m_swapchainImages.size(); i++) {
        m_imageViews.push_back(std::make_unique<ImageView>(device, m_swapchainImages[i], swapchainCreateInfo.imageFormat));
    }
    m_presentQueueFamilyIndex = fldr::vk::utils::presentQueueIndeces(device.physicalDevice().get(),surface.get())[0];
    m_presentQueueHandle = fldr::vk::utils::queueHandle(device.get(), m_presentQueueFamilyIndex);
}

fldr::vk::Swapchain::~Swapchain() {
    vkDestroySwapchainKHR(m_device.get(), m_swapchain, nullptr);
}

VkSwapchainKHR fldr::vk::Swapchain::get() const {
    return m_swapchain;
}
