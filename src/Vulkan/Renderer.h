#pragma once
#include "../Window.h"
#include <vulkan/vulkan.h>
namespace fldr {
    class Renderer {
        public:
            Renderer(const Window & window);
            void addObject();
        private:
            const Window & m_window;
    };
}