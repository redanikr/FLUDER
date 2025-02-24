#pragma once

#include "Instance.h"
#include <vulkan/vulkan_core.h>
namespace FLUDER::Vulkan {
    class Device {
        public:
            Device(Instance);
            VkDevice getDevice();
            VkPhysicalDevice getPhysicalDevice();
        private:
            VkPhysicalDevice m_physicalDevice;
            VkInstance m_instance;
            VkDevice m_device;
    };
}