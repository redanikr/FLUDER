#pragma once

#include "Device.h"
#include <vulkan/vulkan_core.h>
namespace fldr::vk{
    class Semaphore{
        public:
            Semaphore(const Device & device);
            ~Semaphore();
            VkSemaphore get()const;
        private:
            const Device & m_device;
            VkSemaphore m_semaphore;
    };
}