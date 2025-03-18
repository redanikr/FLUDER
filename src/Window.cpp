#include <GLFW/glfw3.h>
#include "Window.h"

fldr::Window::Window() {
    glfwInit();
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    m_window = glfwCreateWindow(900,600,"Hello world",NULL,NULL);
}

fldr::Window::~Window() {
    glfwDestroyWindow(m_window);
    glfwTerminate();
}

GLFWwindow * fldr::Window::get()const {
    return m_window;
}

bool fldr::Window::shouldClose() {
    glfwPollEvents();
    return glfwWindowShouldClose(m_window);
}
