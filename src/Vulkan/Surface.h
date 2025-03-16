#pragma once
#include "Instance.h"
#include "../Window.h"


namespace fldr::vk {
	class Surface {
	public:
		Surface(const Instance & instance, const Window & window);
		~Surface();
		VkSurfaceKHR get()const;
	private:
		const Instance & m_instance;
		VkSurfaceKHR m_surface;
	};
}