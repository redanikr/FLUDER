#pragma once
#include <GLFW/glfw3.h>

namespace FLUDER{
    
    class Window {
        public:
            Window();
            ~Window();
            GLFWwindow * getWindow();
            bool shouldClose();
        private:
            GLFWwindow * m_window;
    };
}