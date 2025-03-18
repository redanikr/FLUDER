#define VK_USE_PLATFORM_WIN32_KHR
#include <GLFW/glfw3.h>
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>
#include <vulkan/vulkan.h>
#include "Surface.h"

fldr::vk::Surface::Surface(const Instance& instance, const Window& window) :
m_instance(instance){
    VkWin32SurfaceCreateInfoKHR win32SurfaceCreateInfoKHR = {};
    win32SurfaceCreateInfoKHR.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
    win32SurfaceCreateInfoKHR.hwnd = glfwGetWin32Window(window.get());
    win32SurfaceCreateInfoKHR.hinstance = GetModuleHandle(nullptr);

    vkCreateWin32SurfaceKHR(m_instance.get(), &win32SurfaceCreateInfoKHR, nullptr, &m_surface);
}

fldr::vk::Surface::~Surface() {
    vkDestroySurfaceKHR(m_instance.get(), m_surface, nullptr);
}

VkSurfaceKHR fldr::vk::Surface::get() const {
	return m_surface;
}
