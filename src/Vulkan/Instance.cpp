#include "Instance.h"
#include "Config/Config.h"
#include "Utils/Utils.h"
fldr::vk::Instance::Instance(bool validationEnabled) : m_validationEnabled(validationEnabled){
	VkApplicationInfo applicationInfo = fldr::vk::config::applicationInfo();
	VkDebugUtilsMessengerCreateInfoEXT debugUtilsMessengerCreateInfoEXT = fldr::vk::config::debugUtilsMessengerCreateInfoEXT();
	std::vector<const char*> extensions = vk::utils::vkInstanceExtensions(true);
	std::vector<const char*> layers = vk::utils::layers();
	VkInstanceCreateInfo instanceCreateInfo = vk::config::instanceCreateInfo(applicationInfo, debugUtilsMessengerCreateInfoEXT, validationEnabled, extensions, layers);
	vkCreateInstance(&instanceCreateInfo, nullptr, &m_instance);
	if (validationEnabled)
		m_debugMessenger= vk::utils::createDebugUtilsMessengerEXT(m_instance, debugUtilsMessengerCreateInfoEXT);
}

fldr::vk::Instance::~Instance() {
	if (m_validationEnabled)
		fldr::vk::utils::destroyDebugUtilsMessengerEXT(m_instance, m_debugMessenger);
	vkDestroyInstance(m_instance, nullptr);
}

VkInstance fldr::vk::Instance::get()const {
	return m_instance;
}
