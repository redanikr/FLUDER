#pragma once

#include "Device.h"
#include <vulkan/vulkan_core.h>
namespace fldr::vk{
    class DescriptorSetLayout{
        public:
        DescriptorSetLayout(const Device & device);
        ~DescriptorSetLayout();
        VkDescriptorSetLayout get()const;
        private:
        const Device & m_device;
        VkDescriptorSetLayout m_descriptorSetLayout;
    };
}