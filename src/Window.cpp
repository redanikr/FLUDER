#include "Window.h"

FLUDER::Window::Window() {
    glfwInit();
    m_window = glfwCreateWindow(900,600,"Hello world",NULL,NULL);
}

FLUDER::Window::~Window() {
    glfwDestroyWindow(m_window);
    glfwTerminate();
}

GLFWwindow *FLUDER::Window::getWindow() {
    return m_window;
}

bool FLUDER::Window::shouldClose() {
    glfwPollEvents();
    return glfwWindowShouldClose(m_window);
}
