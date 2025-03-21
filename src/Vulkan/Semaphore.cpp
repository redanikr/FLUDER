#include <vulkan/vulkan_core.h>
#include "Semaphore.h"

fldr::vk::Semaphore::Semaphore(const Device & device) : m_device(device){
    VkSemaphoreCreateInfo semaphoreCreateInfo = {};
    semaphoreCreateInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
    vkCreateSemaphore(m_device.get(), &semaphoreCreateInfo, nullptr, &m_semaphore);
}
fldr::vk::Semaphore::~Semaphore(){
    vkDeviceWaitIdle(m_device.get());
    vkDestroySemaphore(m_device.get(), m_semaphore, nullptr);
}
VkSemaphore fldr::vk::Semaphore:: get()const{
    return m_semaphore;
}