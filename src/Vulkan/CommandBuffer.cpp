#include "CommandBuffer.h"
#include "Swapchain.h"
#include <vulkan/vulkan.h>
#include "Pipeline.h"
fldr::vk::CommandBuffer::CommandBuffer(const Device& device, const Swapchain & swapchain) : m_device(device){
    VkCommandPoolCreateInfo commandPoolCreateInfo = {};
    commandPoolCreateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    commandPoolCreateInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
    commandPoolCreateInfo.queueFamilyIndex = device.physicalDevice().graphicsQueueFamilyIndex();
    vkCreateCommandPool(m_device.get(), &commandPoolCreateInfo, nullptr, &m_commandPool);

    VkCommandBufferAllocateInfo commandBufferAllocateInfo = {};
    commandBufferAllocateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    commandBufferAllocateInfo.commandPool = m_commandPool;
    commandBufferAllocateInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    commandBufferAllocateInfo.commandBufferCount = 1;

    vkAllocateCommandBuffers(m_device.get(), &commandBufferAllocateInfo, &m_commandBuffer);
}

fldr::vk::CommandBuffer::~CommandBuffer() {
    vkDestroyCommandPool(m_device.get(), m_commandPool, nullptr);
}

void fldr::vk::CommandBuffer::recordCommandBuffer(const ImageView& imageView, const Pipeline& pipeline, VkBuffer vertexBuffer, VkBuffer indexBuffer) {
    VkClearColorValue clearColorValue = {};
    clearColorValue.float32[0] = 0.0f;
    clearColorValue.float32[1] = 0.0f;
    clearColorValue.float32[2] = 0.0f;
    clearColorValue.float32[3] = 0.0f;
    VkClearValue clearValue = {};
    clearValue.color = clearColorValue;

    VkRenderingAttachmentInfo renderingAttachmentInfo = {};
    renderingAttachmentInfo.sType = VK_STRUCTURE_TYPE_RENDERING_ATTACHMENT_INFO;
    renderingAttachmentInfo.imageView = imageView.get();
    renderingAttachmentInfo.clearValue = clearValue;
    renderingAttachmentInfo.imageLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
    renderingAttachmentInfo.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
    renderingAttachmentInfo.storeOp = VK_ATTACHMENT_STORE_OP_STORE;

    VkRenderingInfo renderingInfo = {};
    renderingInfo.sType = VK_STRUCTURE_TYPE_RENDERING_INFO;
    renderingInfo.pColorAttachments = &renderingAttachmentInfo;
    renderingInfo.colorAttachmentCount = 1;
    renderingInfo.layerCount = 1;
    renderingInfo.renderArea = pipeline.scissor();

    VkImageMemoryBarrier2 imageMemoryBarrier1 = {};
    imageMemoryBarrier1.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER_2;
    imageMemoryBarrier1.oldLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    imageMemoryBarrier1.newLayout = VK_IMAGE_LAYOUT_ATTACHMENT_OPTIMAL;
    imageMemoryBarrier1.srcAccessMask = VK_ACCESS_2_NONE;
    imageMemoryBarrier1.dstAccessMask = VK_ACCESS_2_COLOR_ATTACHMENT_WRITE_BIT;
    imageMemoryBarrier1.srcStageMask = VK_PIPELINE_STAGE_2_TOP_OF_PIPE_BIT;
    imageMemoryBarrier1.dstStageMask = VK_PIPELINE_STAGE_2_COLOR_ATTACHMENT_OUTPUT_BIT;
    imageMemoryBarrier1.image = imageView.image();
    imageMemoryBarrier1.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    imageMemoryBarrier1.subresourceRange.levelCount = 1;
    imageMemoryBarrier1.subresourceRange.layerCount = 1;

    VkDependencyInfo dependencyInfo1 = {};
    dependencyInfo1.sType = VK_STRUCTURE_TYPE_DEPENDENCY_INFO;
    dependencyInfo1.imageMemoryBarrierCount = 1;
    dependencyInfo1.pImageMemoryBarriers = &imageMemoryBarrier1;

    VkImageMemoryBarrier2 imageMemoryBarrier2 = {};
    imageMemoryBarrier2.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER_2;
    imageMemoryBarrier2.oldLayout = VK_IMAGE_LAYOUT_ATTACHMENT_OPTIMAL;
    imageMemoryBarrier2.newLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
    imageMemoryBarrier2.srcAccessMask = VK_ACCESS_2_COLOR_ATTACHMENT_WRITE_BIT;
    imageMemoryBarrier2.dstAccessMask = VK_ACCESS_2_NONE;
    imageMemoryBarrier2.srcStageMask = VK_PIPELINE_STAGE_2_COLOR_ATTACHMENT_OUTPUT_BIT;
    imageMemoryBarrier2.dstStageMask = VK_PIPELINE_STAGE_2_BOTTOM_OF_PIPE_BIT;
    imageMemoryBarrier2.image = imageView.image();
    imageMemoryBarrier2.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    imageMemoryBarrier2.subresourceRange.levelCount = 1;
    imageMemoryBarrier2.subresourceRange.layerCount = 1;

    VkDependencyInfo dependencyInfo2 = {};
    dependencyInfo2.sType = VK_STRUCTURE_TYPE_DEPENDENCY_INFO;
    dependencyInfo2.imageMemoryBarrierCount = 1;
    dependencyInfo2.pImageMemoryBarriers = &imageMemoryBarrier2;

    VkCommandBufferBeginInfo commandBufferBeginInfo = {};
    commandBufferBeginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

    VkDeviceSize offsets = 0;

    VkRect2D scissor = pipeline.scissor();

    VkViewport viewport = pipeline.viewport();

    vkResetCommandBuffer(m_commandBuffer, 0);
    vkBeginCommandBuffer(m_commandBuffer, &commandBufferBeginInfo);
    vkCmdPipelineBarrier2(m_commandBuffer, &dependencyInfo1);
    vkCmdBeginRendering(m_commandBuffer, &renderingInfo);
    vkCmdBindVertexBuffers(m_commandBuffer, 0, 1, &vertexBuffer, &offsets);
    vkCmdBindIndexBuffer(m_commandBuffer, indexBuffer, 0, VK_INDEX_TYPE_UINT32);
    vkCmdBindPipeline(m_commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline.get());
    vkCmdSetViewport(m_commandBuffer, 0, 1, &viewport);
    vkCmdSetScissor(m_commandBuffer, 0, 1, &scissor);
    vkCmdDrawIndexed(m_commandBuffer, 6, 1, 0, 0, 0);
    vkCmdEndRendering(m_commandBuffer);
    vkCmdPipelineBarrier2(m_commandBuffer, &dependencyInfo2);
    vkEndCommandBuffer(m_commandBuffer);
}
