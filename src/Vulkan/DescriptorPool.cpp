#include "DescriptorPool.h"
#include <vulkan/vulkan_core.h>


fldr::vk::DescriptorPool::DescriptorPool(const Device & device) :
m_device(device){
    VkDescriptorPoolSize descriptorPoolSize = {};
    descriptorPoolSize.type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    descriptorPoolSize.descriptorCount = 1;

    VkDescriptorPoolCreateInfo descriptorPoolCreateInfo = {};
    descriptorPoolCreateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
    descriptorPoolCreateInfo.poolSizeCount = 1;
    descriptorPoolCreateInfo.pPoolSizes = &descriptorPoolSize;
    descriptorPoolCreateInfo.maxSets = 1;

    vkCreateDescriptorPool(m_device.get(), &descriptorPoolCreateInfo, nullptr, &m_descriptorPool);
}
fldr::vk::DescriptorPool::~DescriptorPool(){
    vkDestroyDescriptorPool(m_device.get(), m_descriptorPool, nullptr);
}
VkDescriptorPool fldr::vk::DescriptorPool::get()const{
    return m_descriptorPool;
}