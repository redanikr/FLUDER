#pragma once

#include "Device.h"
#include <vulkan/vulkan.h>
#include <vector>
namespace fldr::vk {
	template<typename T>
	class Buffer {
	public:
        void addData(const std::vector<T>& data) {
            std::memcpy(&static_cast<T *>(m_data)[m_dataSize], data.data(), data.size() * sizeof(T));
        }
	protected:
        Buffer(const Device& device, VkBufferUsageFlags bufferUsageFlags) :
            m_device(device),
            m_dataSize(0),
            m_maxDataSize(32),
            m_bufferUsageFlags(bufferUsageFlags) {

            VkBufferCreateInfo bufferCreateInfo = {};
            bufferCreateInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
            bufferCreateInfo.size = m_maxDataSize;
            bufferCreateInfo.usage = m_bufferUsageFlags;
            bufferCreateInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

            vkCreateBuffer(m_device.get(), &bufferCreateInfo, nullptr, &m_buffer);

            VkMemoryRequirements memoryRequirements;
            vkGetBufferMemoryRequirements(m_device.get(), m_buffer, &memoryRequirements);

            VkMemoryAllocateInfo memoryAllocateInfo = {};
            memoryAllocateInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
            memoryAllocateInfo.allocationSize = memoryRequirements.size;
            memoryAllocateInfo.memoryTypeIndex = 3;

            vkAllocateMemory(m_device.get(), &memoryAllocateInfo, nullptr, &m_deviceMemory);
            vkBindBufferMemory(m_device.get(), m_buffer, m_deviceMemory, 0);
            vkMapMemory(m_device.get(), m_deviceMemory, 0, bufferCreateInfo.size, 0, &m_data);
        }
        ~Buffer() {
            vkFreeMemory(m_device.get(), m_deviceMemory, nullptr);
            vkDestroyBuffer(m_device.get(), m_buffer, nullptr);
        }
		void createBuffer();
		void destroyBuffer();
		const Device& m_device;
		VkBuffer m_buffer;
		VkDeviceMemory m_deviceMemory;
		void* m_data;
		size_t m_dataSize;
		size_t m_maxDataSize;
		VkBufferUsageFlags m_bufferUsageFlags;
	};
}