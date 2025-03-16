#include <fstream>
#include <ios>
#define VK_USE_PLATFORM_WIN32_KHR
#include "glfw3.h"
#define GLFW_EXPOSE_NATIVE_WIN32
#include "glfw3native.h"
#include <vulkan/vulkan.h>
#include <cstdint>
#include <cstdlib>
#include <exception>
#include <iostream>
#include <vector>
#include "Manager.h"
#include "Utils/Utils.h"
#include "Config/Config.h"

void fldr::vk::Manager::addTriangle(const Triangle & triangle){
    
}
void fldr::vk::Manager::deleteTriangle(const Triangle & triangle){
    
}
fldr::vk::Manager::Manager(const fldr::Window & window, bool validationEnabled) : m_validationEnabled(validationEnabled), m_window(window) {
    
    createInstance();
    choosePhysicalDevice();
    getGraphicsQueueHandle();
    createDevice();
    createSurface();
    createSwapchain();
    createImageViews();
    createShaders();
    createPipeline();
    createCommandBuffer();
    createSemaphoresAndFences();
    createVertexBuffer();
    createIndexBuffer();
}
fldr::vk::Manager::~Manager(){
    vkDeviceWaitIdle(m_device);
    vkDestroySemaphore(m_device, m_imageAcquireSemaphore, nullptr);
    vkDestroySemaphore(m_device, m_renderSemaphore, nullptr);
    vkDestroyFence(m_device, m_fence, nullptr);
    vkDestroyCommandPool(m_device, m_commandPool, nullptr);
    vkDestroyPipelineLayout(m_device, m_pipelineLayout, nullptr);
    vkDestroyPipeline(m_device, m_pipeline, nullptr);
    vkDestroyShaderModule(m_device, m_vertexShaderModule, nullptr);
    vkDestroyShaderModule(m_device, m_fragmentShaderModule, nullptr);
    for (VkImageView imageView : m_swapchainImageViews)
        vkDestroyImageView(m_device, imageView, nullptr);
    vkDestroySwapchainKHR(m_device, m_swapchain, nullptr);
    vkDestroyDescriptorSetLayout(m_device, m_descriptorSetLayout, nullptr);
    vkDestroyBuffer(m_device, m_vertexBuffer, nullptr);
    vkDestroyBuffer(m_device, m_indexBuffer, nullptr);
    vkFreeMemory(m_device, m_vertexBufferMemory, nullptr);
    vkFreeMemory(m_device, m_indexBufferMemory, nullptr);
    vkDestroySurfaceKHR(m_instance, m_surface, nullptr);
    vkDestroyDevice(m_device, nullptr);
    if(m_validationEnabled){
        auto func = (PFN_vkDestroyDebugUtilsMessengerEXT) vkGetInstanceProcAddr(m_instance, "vkDestroyDebugUtilsMessengerEXT");
        func(m_instance, m_debugUtilsMessengerEXT, nullptr);
    }
    vkDestroyInstance(m_instance, nullptr);
}

void fldr::vk::Manager::createInstance(){
    VkApplicationInfo applicationInfo = fldr::vk::config::applicationInfo();
    VkDebugUtilsMessengerCreateInfoEXT debugUtilsMessengerCreateInfoEXT = fldr::vk::config::debugUtilsMessengerCreateInfoEXT();

    std::vector<const char*> extensions = vk::utils::vkInstanceExtensions(true);
    std::vector<const char*> layers = vk::utils::layers();

    VkInstanceCreateInfo instanceCreateInfo = fldr::vk::config::instanceCreateInfo(applicationInfo, debugUtilsMessengerCreateInfoEXT, m_validationEnabled, extensions, layers);

    vkCreateInstance(&instanceCreateInfo,nullptr,&m_instance);

    if(m_validationEnabled){
        auto func = (PFN_vkCreateDebugUtilsMessengerEXT) vkGetInstanceProcAddr(m_instance, "vkCreateDebugUtilsMessengerEXT");
        if (func != nullptr)
            func(m_instance, &debugUtilsMessengerCreateInfoEXT, nullptr, &m_debugUtilsMessengerEXT);
    }
}

void fldr::vk::Manager::choosePhysicalDevice(){
    uint32_t deviceCount = 0;
    vkEnumeratePhysicalDevices(m_instance, &deviceCount, nullptr);
    if(deviceCount == 0) throw std::exception("No GPU found!");
    std::vector<VkPhysicalDevice> devices(deviceCount);
    vkEnumeratePhysicalDevices(m_instance, &deviceCount, devices.data());
    m_physicalDevice = devices[0];
}

void fldr::vk::Manager::createDevice(){
    float queuePriority = 1.0f;
    VkDeviceQueueCreateInfo deviceQueueCreateInfo = {};
    deviceQueueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    deviceQueueCreateInfo.queueFamilyIndex = m_graphicsQueueIndex;
    deviceQueueCreateInfo.queueCount = 1;
    deviceQueueCreateInfo.pQueuePriorities = &queuePriority;

    VkPhysicalDeviceSynchronization2Features physicalDeviceSynchronization2Features = {};
    physicalDeviceSynchronization2Features.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SYNCHRONIZATION_2_FEATURES;
    physicalDeviceSynchronization2Features.synchronization2 = VK_TRUE;
    
    VkPhysicalDeviceDynamicRenderingFeatures physicalDeviceDynamicRenderingFeatures = {};
    physicalDeviceDynamicRenderingFeatures.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_DYNAMIC_RENDERING_FEATURES_KHR;
    physicalDeviceDynamicRenderingFeatures.dynamicRendering = VK_TRUE;
    physicalDeviceDynamicRenderingFeatures.pNext = &physicalDeviceSynchronization2Features;

    VkPhysicalDeviceFeatures2 features = {};
    features.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FEATURES_2;
    vkGetPhysicalDeviceFeatures2(m_physicalDevice, &features);
    features.pNext = &physicalDeviceDynamicRenderingFeatures;
    
    std::vector<const char*> layers = fldr::vk::utils::layers();
    std::vector<const char*> extensions = fldr::vk::utils::vkDeviceExtensions();
    VkDeviceCreateInfo deviceCreateInfo = {};
    deviceCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    deviceCreateInfo.pNext = &features;
    deviceCreateInfo.pQueueCreateInfos = &deviceQueueCreateInfo;
    deviceCreateInfo.queueCreateInfoCount = 1;
    deviceCreateInfo.ppEnabledExtensionNames = extensions.data();
    deviceCreateInfo.enabledExtensionCount = extensions.size();
    deviceCreateInfo.ppEnabledLayerNames = layers.data();
    deviceCreateInfo.enabledLayerCount = layers.size();

    vkCreateDevice(m_physicalDevice, &deviceCreateInfo, nullptr, &m_device);
    
    vkGetDeviceQueue(m_device, m_graphicsQueueIndex, 0, &m_graphicsQueue);
}
void fldr::vk::Manager::getGraphicsQueueHandle(){
    uint32_t queueCount = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(m_physicalDevice, &queueCount,nullptr);
    std::vector<VkQueueFamilyProperties> properties(queueCount);
    vkGetPhysicalDeviceQueueFamilyProperties(m_physicalDevice,&queueCount, properties.data());
    for(int i = 0; i < queueCount; i++){
        if(properties[i].queueFlags & VK_QUEUE_GRAPHICS_BIT){
            m_graphicsQueueIndex = i;
            break;
        }
    }
}
void fldr::vk::Manager::createSurface(){
    VkWin32SurfaceCreateInfoKHR win32SurfaceCreateInfoKHR = {};
    win32SurfaceCreateInfoKHR.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
    win32SurfaceCreateInfoKHR.hwnd = glfwGetWin32Window(m_window.getWindow());
    win32SurfaceCreateInfoKHR.hinstance = GetModuleHandle(nullptr);
    
    m_presentQueue = m_graphicsQueue;

    vkCreateWin32SurfaceKHR(m_instance, &win32SurfaceCreateInfoKHR, nullptr, &m_surface);

}
void fldr::vk::Manager::createSwapchain(){
    VkSurfaceCapabilitiesKHR surfaceCapabilitiesKHR;
    vkGetPhysicalDeviceSurfaceCapabilitiesKHR(m_physicalDevice, m_surface, &surfaceCapabilitiesKHR);
    uint32_t physicalDeviceSurfaceFormatCount = 0;
    vkGetPhysicalDeviceSurfaceFormatsKHR(m_physicalDevice, m_surface, &physicalDeviceSurfaceFormatCount, nullptr);
    std::vector<VkSurfaceFormatKHR> surfaceFormats(physicalDeviceSurfaceFormatCount);
    vkGetPhysicalDeviceSurfaceFormatsKHR(m_physicalDevice, m_surface, &physicalDeviceSurfaceFormatCount, surfaceFormats.data());


    m_extent2D = surfaceCapabilitiesKHR.currentExtent;
    m_presentModeKHR = VK_PRESENT_MODE_IMMEDIATE_KHR;
    m_surfaceFormatKHR = surfaceFormats[0];
    m_swapchainImageCount = surfaceCapabilitiesKHR.minImageCount + 1;

    VkSwapchainCreateInfoKHR swapchainCreateInfoKHR = {};
    swapchainCreateInfoKHR.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    swapchainCreateInfoKHR.surface = m_surface;
    swapchainCreateInfoKHR.minImageCount = m_swapchainImageCount;
    swapchainCreateInfoKHR.imageFormat = m_surfaceFormatKHR.format;
    swapchainCreateInfoKHR.imageColorSpace = m_surfaceFormatKHR.colorSpace;
    swapchainCreateInfoKHR.imageExtent = m_extent2D;
    swapchainCreateInfoKHR.imageArrayLayers = 1;
    swapchainCreateInfoKHR.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
    swapchainCreateInfoKHR.preTransform = surfaceCapabilitiesKHR.currentTransform;
    swapchainCreateInfoKHR.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
    swapchainCreateInfoKHR.presentMode = m_presentModeKHR;
    swapchainCreateInfoKHR.oldSwapchain = VK_NULL_HANDLE;
    vkCreateSwapchainKHR(m_device, &swapchainCreateInfoKHR, nullptr, &m_swapchain);
    uint32_t swapchainImageCount = 0;
    vkGetSwapchainImagesKHR(m_device, m_swapchain, &swapchainImageCount, nullptr);
    m_swapchainImages.resize(swapchainImageCount);
    vkGetSwapchainImagesKHR(m_device, m_swapchain, &swapchainImageCount, m_swapchainImages.data());
}
void fldr::vk::Manager::createImageViews(){
    m_swapchainImageViews.resize(m_swapchainImageCount);
    for(int i = 0; i < m_swapchainImageCount; i++){
        VkImageViewCreateInfo imageViewCreateInfo = {};
        imageViewCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
        imageViewCreateInfo.image = m_swapchainImages[i];
        imageViewCreateInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
        imageViewCreateInfo.format = m_surfaceFormatKHR.format;
        imageViewCreateInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
        imageViewCreateInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
        imageViewCreateInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
        imageViewCreateInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
        imageViewCreateInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        imageViewCreateInfo.subresourceRange.baseMipLevel = 0;
        imageViewCreateInfo.subresourceRange.levelCount = 1;
        imageViewCreateInfo.subresourceRange.baseArrayLayer = 0;
        imageViewCreateInfo.subresourceRange.layerCount = 1;
        vkCreateImageView(m_device, &imageViewCreateInfo, nullptr, &m_swapchainImageViews[i]);
    }
}
void fldr::vk::Manager::createShaders(){
    system("glslc.exe ../../../shaders/shader.vert -o vert.spv");
    system("glslc.exe ../../../shaders/shader.frag -o frag.spv");
    std::vector<char> vertexShaderCode;
    std::vector<char> fragmentShaderCode;
    std::ifstream file1 = std::ifstream("vert.spv",std::ios_base::binary);
    std::ifstream file2 = std::ifstream("frag.spv",std::ios_base::binary);
    while(1){
        int byte = file1.get();
        if(byte == - 1)
            break;
        vertexShaderCode.push_back(byte);
    }
    while(1){
        int byte = file2.get();
        if(byte == - 1)
            break;
        fragmentShaderCode.push_back(byte);
    }
    VkShaderModuleCreateInfo vertexShaderModuleCreateInfo = {};
    vertexShaderModuleCreateInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    vertexShaderModuleCreateInfo.pCode = reinterpret_cast<uint32_t * >(vertexShaderCode.data());
    vertexShaderModuleCreateInfo.codeSize = vertexShaderCode.size();

    VkShaderModuleCreateInfo fragmentShaderModuleCreateInfo = {};
    fragmentShaderModuleCreateInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    fragmentShaderModuleCreateInfo.pCode = reinterpret_cast<uint32_t * >(fragmentShaderCode.data());
    fragmentShaderModuleCreateInfo.codeSize = fragmentShaderCode.size();

    vkCreateShaderModule(m_device, &vertexShaderModuleCreateInfo, nullptr, &m_vertexShaderModule);
    vkCreateShaderModule(m_device, &fragmentShaderModuleCreateInfo, nullptr, &m_fragmentShaderModule);
}

void fldr::vk::Manager::createPipeline(){
    std::vector<VkDynamicState> dynamicStates = {VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_SCISSOR};

    VkPipelineDynamicStateCreateInfo pipelineDynamicStateCreateInfo = {};
    pipelineDynamicStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
    pipelineDynamicStateCreateInfo.pDynamicStates = dynamicStates.data();
    pipelineDynamicStateCreateInfo.dynamicStateCount = dynamicStates.size();

    VkVertexInputBindingDescription vertexInputBindingDescription = {};
    vertexInputBindingDescription.binding = 0;
    vertexInputBindingDescription.stride = 12;
    vertexInputBindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

    VkVertexInputAttributeDescription vertexInputAttributeDescription = {};
    vertexInputAttributeDescription.binding = 0;
    vertexInputAttributeDescription.location = 0;
    vertexInputAttributeDescription.format = VK_FORMAT_R32G32B32_SFLOAT;
    vertexInputAttributeDescription.offset = 0;

    VkPipelineVertexInputStateCreateInfo pipelineVertexInputStateCreateInfo = {};
    pipelineVertexInputStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
    pipelineVertexInputStateCreateInfo.pVertexAttributeDescriptions = &vertexInputAttributeDescription;
    pipelineVertexInputStateCreateInfo.vertexAttributeDescriptionCount = 1;
    pipelineVertexInputStateCreateInfo.pVertexBindingDescriptions = &vertexInputBindingDescription;
    pipelineVertexInputStateCreateInfo.vertexBindingDescriptionCount = 1;

    VkDescriptorSetLayoutBinding descriptorSetLayoutBinding = {};
    descriptorSetLayoutBinding.binding = 0;
    descriptorSetLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    descriptorSetLayoutBinding.descriptorCount = 1;
    descriptorSetLayoutBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;

    VkDescriptorSetLayoutCreateInfo descriptorSetLayoutCreateInfo = {};
    descriptorSetLayoutCreateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
    descriptorSetLayoutCreateInfo.bindingCount = 1;
    descriptorSetLayoutCreateInfo.pBindings = &descriptorSetLayoutBinding;

    vkCreateDescriptorSetLayout(m_device, &descriptorSetLayoutCreateInfo, nullptr, &m_descriptorSetLayout);

    VkPipelineInputAssemblyStateCreateInfo pipelineInputAssemblyStateCreateInfo = {};
    pipelineInputAssemblyStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
    pipelineInputAssemblyStateCreateInfo.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
    pipelineInputAssemblyStateCreateInfo.primitiveRestartEnable = VK_FALSE;

    m_viewport = {};
    m_viewport.width = m_extent2D.width;
    m_viewport.height = m_extent2D.height;

    m_scissor = {};
    m_scissor.offset.x = 0;
    m_scissor.offset.y = 0;
    m_scissor.extent = m_extent2D;

    VkPipelineRasterizationStateCreateInfo pipelineRasterizationStateCreateInfo = {};
    pipelineRasterizationStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
    pipelineRasterizationStateCreateInfo.depthClampEnable = VK_FALSE;
    pipelineRasterizationStateCreateInfo.rasterizerDiscardEnable = VK_FALSE;
    pipelineRasterizationStateCreateInfo.polygonMode = VK_POLYGON_MODE_FILL;
    pipelineRasterizationStateCreateInfo.cullMode = VK_CULL_MODE_BACK_BIT;
    pipelineRasterizationStateCreateInfo.frontFace = VK_FRONT_FACE_CLOCKWISE;
    pipelineRasterizationStateCreateInfo.depthBiasEnable = VK_FALSE;
    pipelineRasterizationStateCreateInfo.lineWidth = 1.0f;

    VkPipelineMultisampleStateCreateInfo pipelineMultisampleStateCreateInfo = {};
    pipelineMultisampleStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
    pipelineMultisampleStateCreateInfo.sampleShadingEnable = VK_FALSE;
    pipelineMultisampleStateCreateInfo.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;

    VkPipelineColorBlendAttachmentState pipelineColorBlendAttachmentState = {};
    pipelineColorBlendAttachmentState.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
    pipelineColorBlendAttachmentState.blendEnable = VK_FALSE;

    VkPipelineColorBlendStateCreateInfo pipelineColorBlendStateCreateInfo = {};
    pipelineColorBlendStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
    pipelineColorBlendStateCreateInfo.logicOpEnable = VK_FALSE;
    pipelineColorBlendStateCreateInfo.attachmentCount = 1;
    pipelineColorBlendStateCreateInfo.pAttachments = &pipelineColorBlendAttachmentState;

    VkPipelineLayoutCreateInfo pipelineLayoutCreateInfo = {};
    pipelineLayoutCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    pipelineLayoutCreateInfo.setLayoutCount = 1;
    pipelineLayoutCreateInfo.pSetLayouts = &m_descriptorSetLayout;

    vkCreatePipelineLayout(m_device, &pipelineLayoutCreateInfo, nullptr, &m_pipelineLayout);

    VkPipelineShaderStageCreateInfo pipelineVertexShaderStageCreateInfo = {};
    pipelineVertexShaderStageCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    pipelineVertexShaderStageCreateInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
    pipelineVertexShaderStageCreateInfo.module = m_vertexShaderModule;
    pipelineVertexShaderStageCreateInfo.pName = "main";

    VkPipelineShaderStageCreateInfo pipelineFragmentShaderStageCreateInfo = {};
    pipelineFragmentShaderStageCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    pipelineFragmentShaderStageCreateInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
    pipelineFragmentShaderStageCreateInfo.module = m_fragmentShaderModule;
    pipelineFragmentShaderStageCreateInfo.pName = "main";

    VkPipelineViewportStateCreateInfo pipelineViewportStateCreateInfo = {};
    pipelineViewportStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
    pipelineViewportStateCreateInfo.viewportCount = 1;
    pipelineViewportStateCreateInfo.scissorCount = 1;

    std::vector<VkPipelineShaderStageCreateInfo> shaderStages = {pipelineVertexShaderStageCreateInfo,pipelineFragmentShaderStageCreateInfo};

    VkPipelineRenderingCreateInfoKHR pipelineRenderingCreateInfoKHR = {};
    pipelineRenderingCreateInfoKHR.sType = VK_STRUCTURE_TYPE_PIPELINE_RENDERING_CREATE_INFO_KHR;
    pipelineRenderingCreateInfoKHR.colorAttachmentCount = 1;
    pipelineRenderingCreateInfoKHR.pColorAttachmentFormats = &m_surfaceFormatKHR.format;

    VkGraphicsPipelineCreateInfo graphicsPipelineCreateInfo = {};
    graphicsPipelineCreateInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
    graphicsPipelineCreateInfo.pNext = &pipelineRenderingCreateInfoKHR;
    graphicsPipelineCreateInfo.stageCount = shaderStages.size();
    graphicsPipelineCreateInfo.pStages = shaderStages.data();
    graphicsPipelineCreateInfo.pVertexInputState = &pipelineVertexInputStateCreateInfo;
    graphicsPipelineCreateInfo.pInputAssemblyState = &pipelineInputAssemblyStateCreateInfo;
    graphicsPipelineCreateInfo.pViewportState = &pipelineViewportStateCreateInfo;
    graphicsPipelineCreateInfo.pRasterizationState = &pipelineRasterizationStateCreateInfo;
    graphicsPipelineCreateInfo.pMultisampleState = &pipelineMultisampleStateCreateInfo;
    graphicsPipelineCreateInfo.pDepthStencilState = nullptr;
    graphicsPipelineCreateInfo.pColorBlendState = &pipelineColorBlendStateCreateInfo;
    graphicsPipelineCreateInfo.pDynamicState = &pipelineDynamicStateCreateInfo;
    graphicsPipelineCreateInfo.layout = m_pipelineLayout;

    vkCreateGraphicsPipelines(m_device, VK_NULL_HANDLE, 1, &graphicsPipelineCreateInfo, nullptr, &m_pipeline);

}
void fldr::vk::Manager::createCommandBuffer(){
    VkCommandPoolCreateInfo commandPoolCreateInfo = {};
    commandPoolCreateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    commandPoolCreateInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
    commandPoolCreateInfo.queueFamilyIndex = m_graphicsQueueIndex;
    vkCreateCommandPool(m_device, &commandPoolCreateInfo, nullptr, &m_commandPool);

    VkCommandBufferAllocateInfo commandBufferAllocateInfo = {};
    commandBufferAllocateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    commandBufferAllocateInfo.commandPool = m_commandPool;
    commandBufferAllocateInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    commandBufferAllocateInfo.commandBufferCount = 1;

    vkAllocateCommandBuffers(m_device, &commandBufferAllocateInfo, &m_commandBuffer);
}
void fldr::vk::Manager::createSemaphoresAndFences(){
    VkSemaphoreCreateInfo semaphoreCreateInfo = {};
    semaphoreCreateInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

    vkCreateSemaphore(m_device, &semaphoreCreateInfo, nullptr, &m_renderSemaphore);
    vkCreateSemaphore(m_device, &semaphoreCreateInfo, nullptr, &m_imageAcquireSemaphore);
    
    VkFenceCreateInfo fenceCreateInfo = {};
    fenceCreateInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
    fenceCreateInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

    vkCreateFence(m_device, &fenceCreateInfo, nullptr, &m_fence);
}
void fldr::vk::Manager::recordCommandBuffer(){
    VkClearColorValue clearColorValue = {};
    clearColorValue.float32[0] = 0.0f;
    clearColorValue.float32[1] = 0.0f;
    clearColorValue.float32[2] = 0.0f;
    clearColorValue.float32[3] = 0.0f;
    VkClearValue clearValue = {};
    clearValue.color = clearColorValue;

    VkRenderingAttachmentInfo renderingAttachmentInfo = {};
    renderingAttachmentInfo.sType = VK_STRUCTURE_TYPE_RENDERING_ATTACHMENT_INFO;
    renderingAttachmentInfo.imageView = m_swapchainImageViews[m_imageReadyIndex];
    renderingAttachmentInfo.clearValue = clearValue;
    renderingAttachmentInfo.imageLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
    renderingAttachmentInfo.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
    renderingAttachmentInfo.storeOp = VK_ATTACHMENT_STORE_OP_STORE;

    VkRect2D rect2D = {};
    rect2D.extent = m_extent2D;

    VkRenderingInfo renderingInfo = {};
    renderingInfo.sType = VK_STRUCTURE_TYPE_RENDERING_INFO;
    renderingInfo.pColorAttachments = &renderingAttachmentInfo;
    renderingInfo.colorAttachmentCount = 1;
    renderingInfo.layerCount = 1;
    renderingInfo.renderArea = rect2D;

    VkImageMemoryBarrier2 imageMemoryBarrier1 = {};
    imageMemoryBarrier1.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER_2;
    imageMemoryBarrier1.oldLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    imageMemoryBarrier1.newLayout = VK_IMAGE_LAYOUT_ATTACHMENT_OPTIMAL;
    imageMemoryBarrier1.srcAccessMask = VK_ACCESS_2_NONE;
    imageMemoryBarrier1.dstAccessMask = VK_ACCESS_2_COLOR_ATTACHMENT_WRITE_BIT;
    imageMemoryBarrier1.srcStageMask = VK_PIPELINE_STAGE_2_TOP_OF_PIPE_BIT;
    imageMemoryBarrier1.dstStageMask = VK_PIPELINE_STAGE_2_COLOR_ATTACHMENT_OUTPUT_BIT;
    imageMemoryBarrier1.image = m_swapchainImages[m_imageReadyIndex];
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
    imageMemoryBarrier2.image = m_swapchainImages[m_imageReadyIndex];
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

    vkResetCommandBuffer(m_commandBuffer, 0);
    vkBeginCommandBuffer(m_commandBuffer, &commandBufferBeginInfo);
    vkCmdPipelineBarrier2(m_commandBuffer, &dependencyInfo1);
    vkCmdBeginRendering(m_commandBuffer, &renderingInfo);
    vkCmdBindVertexBuffers(m_commandBuffer, 0, 1, &m_vertexBuffer,&offsets);
    vkCmdBindIndexBuffer(m_commandBuffer, m_indexBuffer, 0, VK_INDEX_TYPE_UINT32);
    vkCmdBindPipeline(m_commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, m_pipeline);
    vkCmdSetViewport(m_commandBuffer, 0, 1, &m_viewport);
    vkCmdSetScissor(m_commandBuffer, 0, 1, &m_scissor);
    vkCmdDrawIndexed(m_commandBuffer, 6, 1, 0, 0,0);
    vkCmdEndRendering(m_commandBuffer);
    vkCmdPipelineBarrier2(m_commandBuffer, &dependencyInfo2);
    vkEndCommandBuffer(m_commandBuffer);
}
void fldr::vk::Manager::draw(){
    vkWaitForFences(m_device, 1, &m_fence, VK_TRUE, UINT64_MAX);
    vkResetFences(m_device, 1, &m_fence);
    vkAcquireNextImageKHR(m_device, m_swapchain, UINT64_MAX, m_imageAcquireSemaphore, VK_NULL_HANDLE, &m_imageReadyIndex);
    
    recordCommandBuffer();
    
    VkPipelineStageFlags pipelineStageFlags = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    VkSubmitInfo submitInfo = {};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submitInfo.pCommandBuffers = &m_commandBuffer;
    submitInfo.commandBufferCount = 1;
    submitInfo.pSignalSemaphores = &m_renderSemaphore;
    submitInfo.signalSemaphoreCount = 1;
    submitInfo.pWaitSemaphores = &m_imageAcquireSemaphore;
    submitInfo.waitSemaphoreCount = 1;
    submitInfo.pWaitDstStageMask = &pipelineStageFlags;

    vkQueueSubmit(m_graphicsQueue, 1, &submitInfo, m_fence);
    VkPresentInfoKHR presentInfo = {};
    presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
    presentInfo.pWaitSemaphores = &m_renderSemaphore;
    presentInfo.waitSemaphoreCount = 1;
    presentInfo.swapchainCount = 1;
    presentInfo.pSwapchains = &m_swapchain;
    presentInfo.pImageIndices = &m_imageReadyIndex;
    vkQueuePresentKHR(m_presentQueue, &presentInfo);
}
void fldr::vk::Manager::createVertexBuffer(){
    VkBufferCreateInfo bufferCreateInfo = {};
    bufferCreateInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    bufferCreateInfo.size = 12 * 4;
    bufferCreateInfo.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
    bufferCreateInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

    vkCreateBuffer(m_device, &bufferCreateInfo, nullptr, &m_vertexBuffer);

    VkMemoryRequirements memoryRequirements;
    vkGetBufferMemoryRequirements(m_device, m_vertexBuffer, &memoryRequirements);

    VkMemoryAllocateInfo memoryAllocateInfo = {};
    memoryAllocateInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    memoryAllocateInfo.allocationSize = memoryRequirements.size;
    memoryAllocateInfo.memoryTypeIndex = 3;

    vkAllocateMemory(m_device, &memoryAllocateInfo, nullptr, &m_vertexBufferMemory);

    vkBindBufferMemory(m_device, m_vertexBuffer, m_vertexBufferMemory, 0);

    void * data = new char[128];

    vkMapMemory(m_device, m_vertexBufferMemory, 0, bufferCreateInfo.size, 0, &data);

    ((float *) data)[0] = -0.9f;
    ((float *) data)[1] = 0.8f;
    ((float *) data)[2] = 0.0f;
    ((float *) data)[3] = 0.5f;
    ((float *) data)[4] = 0.5f;
    ((float *) data)[5] = 0.0f;
    ((float *) data)[6] = 0.8f;
    ((float *) data)[7] = -0.8f;
    ((float *) data)[8] = 0.5f;
    ((float *) data)[9] = 0.0f;
    ((float *) data)[10] = 0.9f;
    ((float *) data)[11] = 0.0f;
}
void fldr::vk::Manager::createIndexBuffer(){
    VkBufferCreateInfo bufferCreateInfo = {};
    bufferCreateInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    bufferCreateInfo.size = VkDeviceSize(4 * 6);
    bufferCreateInfo.usage = VK_BUFFER_USAGE_INDEX_BUFFER_BIT;
    bufferCreateInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
    
    vkCreateBuffer(m_device, &bufferCreateInfo, nullptr, &m_indexBuffer);

    VkMemoryRequirements memoryRequirements;
    vkGetBufferMemoryRequirements(m_device, m_indexBuffer, &memoryRequirements);

    VkMemoryAllocateInfo memoryAllocateInfo = {};
    memoryAllocateInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    memoryAllocateInfo.allocationSize = memoryRequirements.size;
    memoryAllocateInfo.memoryTypeIndex = 3;

    vkAllocateMemory(m_device, &memoryAllocateInfo, nullptr, &m_indexBufferMemory);

    vkBindBufferMemory(m_device, m_indexBuffer, m_indexBufferMemory, 0);

    void * data = new char[128];

    vkMapMemory(m_device, m_indexBufferMemory, 0, bufferCreateInfo.size, 0, &data);

    ((uint32_t *) data)[0] = 2;
    ((uint32_t *) data)[1] = 1;
    ((uint32_t *) data)[2] = 0;
    ((uint32_t *) data)[3] = 0;
    ((uint32_t *) data)[4] = 1;
    ((uint32_t *) data)[5] = 3;
}
