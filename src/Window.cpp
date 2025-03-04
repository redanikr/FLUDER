#include "Window.h"
#include "glfw3.h"

FLUDER::Window::Window() {
    glfwInit();
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
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
