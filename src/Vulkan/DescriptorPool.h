#pragma once

#include "Device.h"
#include <vulkan/vulkan_core.h>
namespace fldr::vk {
    class DescriptorPool{
        public:
        DescriptorPool(const Device & device);
        ~DescriptorPool();
        VkDescriptorPool get()const;
        private:
        const Device & m_device;
        VkDescriptorPool m_descriptorPool;
    };
}