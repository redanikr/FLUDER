#include <vector>
#include "DescriptorSet.h"
#include "DescriptorPool.h"
#include "DescriptorSetLayout.h"
#include <vulkan/vulkan_core.h>

fldr::vk::DescriptorSet::DescriptorSet(const Device & device) :
m_device(device),
m_descriptorPool(m_device),
m_descriptorSetLayout(m_device){

    std::vector<VkDescriptorSetLayout> layouts{m_descriptorSetLayout.get()};

    VkDescriptorSetAllocateInfo descriptorSetAllocateInfo = {};
    descriptorSetAllocateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
    descriptorSetAllocateInfo.descriptorPool = m_descriptorPool.get();
    descriptorSetAllocateInfo.descriptorSetCount = 1;
    descriptorSetAllocateInfo.pSetLayouts = layouts.data();

    vkAllocateDescriptorSets(m_device.get(), &descriptorSetAllocateInfo, &m_descriptorSet);

}