#include "DescriptorSetLayout.h"
#include <vulkan/vulkan_core.h>

fldr::vk::DescriptorSetLayout::DescriptorSetLayout(const Device & device) :
m_device(device){
    VkDescriptorSetLayoutBinding descriptorSetLayoutBinding = {};
    descriptorSetLayoutBinding.binding = 0;
    descriptorSetLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    descriptorSetLayoutBinding.descriptorCount = 1;
    descriptorSetLayoutBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;

    VkDescriptorSetLayoutCreateInfo descriptorSetLayoutCreateInfo = {};
    descriptorSetLayoutCreateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
    descriptorSetLayoutCreateInfo.bindingCount = 1;
    descriptorSetLayoutCreateInfo.pBindings = &descriptorSetLayoutBinding;

    vkCreateDescriptorSetLayout(m_device.get(), &descriptorSetLayoutCreateInfo, nullptr, &m_descriptorSetLayout);
}
fldr::vk::DescriptorSetLayout::~DescriptorSetLayout(){
    vkDestroyDescriptorSetLayout(m_device.get(), m_descriptorSetLayout, nullptr);
}
VkDescriptorSetLayout fldr::vk::DescriptorSetLayout::get()const{
    return m_descriptorSetLayout;
}