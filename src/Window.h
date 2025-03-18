#pragma once
#include <GLFW/glfw3.h>

namespace fldr{
    
    class Window {
        public:
            Window();
            ~Window();
            GLFWwindow * get()const;
            bool shouldClose();
        private:
            GLFWwindow * m_window;
    };
}