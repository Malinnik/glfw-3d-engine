#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "window.h"
#include "../config.h"
#include <cstdlib>
#include <iostream>

Window::Window() {
    if (!glfwInit())
        exit(-1);

    if (FULLSCREEN)
        _window = glfwCreateWindow(WIDTH, HEIGHT, "3D-Engine",
                                glfwGetPrimaryMonitor(), NULL);
    else
        _window = glfwCreateWindow(WIDTH, HEIGHT, "3D-Engine", NULL, NULL);
 
    if (!_window) {
        glfwTerminate();
        exit(-1);
    }
    glfwMakeContextCurrent(_window);
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        exit(-1);
    }
    glfwSetFramebufferSizeCallback(_window, framebuffer_size_callback);
    if (WIREFRAME)
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
}

Window::~Window() {
    glfwTerminate();
}

GLFWwindow* Window::getWindow() {
    return _window;
}

bool Window::isShouldClose()
{
    return glfwWindowShouldClose(_window);
}

void Window::swapBuffers()
{
    glfwSwapBuffers(_window);
}

void Window::render()
{
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
}

void Window::framebuffer_size_callback(GLFWwindow *window, int width, int height) 
{
    glViewport(0, 0, width, height);
}