#include "Fence.h"
#include <cstdint>
#include <vulkan/vulkan_core.h>

fldr::vk::Fence::Fence(const Device & device) : m_device(device){
    VkFenceCreateInfo fenceCreateInfo = {};
    fenceCreateInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
    fenceCreateInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

    vkCreateFence(m_device.get(), &fenceCreateInfo, nullptr, &m_fence);
}
fldr::vk::Fence::~Fence(){
    vkDeviceWaitIdle(m_device.get());
    vkDestroyFence(m_device.get(), m_fence, nullptr);
}
VkFence fldr::vk::Fence::get()const{
    return m_fence;
}
void fldr::vk::Fence::reset()const{
    vkResetFences(m_device.get(), 1, &m_fence);
}
void fldr::vk::Fence::wait()const{
    vkWaitForFences(m_device.get(), 1, &m_fence, VK_TRUE, UINT64_MAX);
}