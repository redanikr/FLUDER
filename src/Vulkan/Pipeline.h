#pragma once
#include "Device.h"
#include "Shader.h"
#include "Surface.h"
#include <vulkan/vulkan.h>

namespace fldr::vk {
	class Pipeline {
	public:
		Pipeline(const Device & device, const Surface& surface, const Shader & vertexShader, const Shader & fragmentShader);
		~Pipeline();
		VkPipeline get()const;
		VkViewport viewport()const;
		VkRect2D scissor()const;

	private:
		const Device& m_device;
		VkPipeline m_pipeline;
		VkPipelineLayout m_pipelineLayout;
		VkDescriptorSetLayout m_descriptorSetLayout;
		VkViewport m_viewport;
		VkRect2D m_scissor;
	};
}