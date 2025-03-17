#include "Shader.h"
#include <vector>
#include <ios>
#include <fstream>
#include <sstream>
#include <exception>
#include "Utils/Utils.h"
fldr::vk::Shader::Shader(const Device& device, std::string shaderPath) : m_device(device) {
    int hash = std::hash<std::string>{}(shaderPath);
    std::stringstream stream;
    stream << std::hex << hash;
    std::string command = "glslc.exe " + shaderPath + " -o " + std::string(stream.str()) + ".spv";
    if(system(command.c_str()) != 0)
        throw std::exception("Shader file not found!");

    std::vector<char> shaderCode = fldr::vk::utils::readShaderFile(std::string(stream.str() + ".spv"));
    VkShaderModuleCreateInfo shaderModuleCreateInfo = {};
    shaderModuleCreateInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    shaderModuleCreateInfo.pCode = reinterpret_cast<uint32_t*>(shaderCode.data());
    shaderModuleCreateInfo.codeSize = shaderCode.size();

    vkCreateShaderModule(m_device.get(), &shaderModuleCreateInfo, nullptr, &m_shaderModule);
}

fldr::vk::Shader::~Shader() {
    vkDestroyShaderModule(m_device.get(),m_shaderModule,nullptr);
}

VkShaderModule fldr::vk::Shader::get() const {
    return m_shaderModule;
}
