#pragma once
#include "Device.h"
#include "Swapchain.h"
#include "Pipeline.h"

namespace fldr::vk {
	class CommandBuffer {
	public:
		CommandBuffer(const Device& device, const Swapchain& swapchain);
		~CommandBuffer();
		void recordCommandBuffer(const ImageView& imageView, const Pipeline& pipeline, VkBuffer vertexBuffer, VkBuffer indexBuffer);
	private:
		const Device& m_device;
		VkCommandBuffer m_commandBuffer;
		VkCommandPool m_commandPool;
	};
}