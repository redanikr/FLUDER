#pragma once

#include <vulkan/vulkan_core.h>
#include "DescriptorPool.h"
#include "DescriptorSetLayout.h"
#include "Device.h"
namespace fldr::vk{
    class DescriptorSet{
        public:
        DescriptorSet(const Device & device);
        private:
        const Device & m_device;
        DescriptorPool m_descriptorPool;
        DescriptorSetLayout m_descriptorSetLayout;
        VkDescriptorSet m_descriptorSet;
    };
}