#pragma once

#include "Device.h"
#include <vulkan/vulkan_core.h>
namespace fldr::vk{
    class Fence{
        public:
            Fence(const Device & device);
            ~Fence();
            VkFence get()const;
            void reset()const;
            void wait()const;
        private:
            const Device & m_device;
            VkFence m_fence;
    };
}