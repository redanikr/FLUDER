#pragma once
#include <vulkan/vulkan.h>
namespace fldr::vk {
	class Instance {
	public:
		Instance(bool validationEnabled);
		~Instance();
		VkInstance get()const;
	private:
		VkInstance m_instance;
		VkDebugUtilsMessengerEXT m_debugMessenger;
		bool m_validationEnabled;
	};
}