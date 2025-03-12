#pragma once

#include <cstdint>
#include <vulkan/vulkan_core.h>
#include <vector>
#include "../Window.h"
#include "../Triangle.h"

namespace FLUDER::Vulkan {
    class VulkanManager{
        public:   
        static VKAPI_ATTR VkBool32 VKAPI_CALL debugUtilsMessageCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,VkDebugUtilsMessageTypeFlagsEXT messageType,const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,void* pUserData);
        VulkanManager(const FLUDER::Window & window, bool validationEnabled);
        ~VulkanManager();
        void draw();
        void addTriangle(const Triangle & triangle);
        void deleteTriangle(const Triangle & triangle);
        private:
        void createInstance();
        void choosePhysicalDevice();
        void createDevice();
        void getGraphicsQueueHandle();
        void createSurface();
        void createSwapchain();
        void createImageViews();
        void createShaders();
        void createPipeline();
        void createCommandBuffer();
        void createSemaphoresAndFences();
        void recordCommandBuffer();
        void createVertexBuffer();
        void createIndexBuffer();

        bool m_validationEnabled;
        Window m_window;
        VkInstance m_instance;
        VkDevice m_device;
        VkDebugUtilsMessengerEXT m_debugUtilsMessengerEXT;
        VkPhysicalDevice m_physicalDevice;
        uint32_t m_graphicsQueueIndex;
        VkQueue m_graphicsQueue;
        VkQueue m_presentQueue;
        VkSurfaceKHR m_surface;
        VkSurfaceFormatKHR m_surfaceFormatKHR;
        VkPresentModeKHR m_presentModeKHR;
        VkExtent2D m_extent2D;
        uint32_t m_swapchainImageCount;
        VkSwapchainKHR m_swapchain;
        std::vector<VkImage> m_swapchainImages;
        std::vector<VkImageView> m_swapchainImageViews;
        VkShaderModule m_vertexShaderModule;
        VkShaderModule m_fragmentShaderModule;
        VkPipelineLayout m_pipelineLayout;
        VkPipeline m_pipeline;
        VkCommandPool m_commandPool;
        VkCommandBuffer m_commandBuffer;
        VkRect2D m_scissor;
        VkViewport m_viewport;
        VkSemaphore m_imageAcquireSemaphore;
        VkSemaphore m_renderSemaphore;
        VkFence m_fence;
        uint32_t m_imageReadyIndex;
        VkBuffer m_vertexBuffer;
        VkBuffer m_indexBuffer;
        VkDeviceMemory m_indexBufferMemory;
        VkDeviceMemory m_vertexBufferMemory;
        VkDescriptorSetLayout m_descriptorSetLayout;
        uint32_t m_vertexCount = 1;
        uint32_t m_indexCount = 1;

        static const std::vector<const char *> instanceExtensions;
        static const std::vector<const char *> deviceExtensions;
        static const std::vector<const char *> enabledLayers;
    };
}