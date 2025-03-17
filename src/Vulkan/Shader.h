#pragma once
#include <string>
#include <vulkan/vulkan.h>
#include "Device.h"
namespace fldr::vk {
    class Shader {
    public:
        Shader(const Device& device, std::string shaderPath);
        ~Shader();
        VkShaderModule get() const;
    private:
        const Device& m_device;
        VkShaderModule m_shaderModule;

    };

}
