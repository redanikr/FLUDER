#pragma once
#include "Window.h"
namespace FLUDER {
    class Renderer {
        public:
            Renderer(const Window & window);
            void renderTriangle();
        private:
            const Window & m_window;
    };
}