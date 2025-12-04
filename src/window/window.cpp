#include <cstdlib>
#include <iostream>

#include "glad/glad.h"
#include "GLFW/glfw3.h"

#include "window.h"
#include "config.h"

GLFWwindow* Window::window;

Window::Window() {
    if (!glfwInit())
        exit(-1);

    if (FULLSCREEN)
        window = glfwCreateWindow(WIDTH, HEIGHT, "3D-Engine",
                                glfwGetPrimaryMonitor(), NULL);
    else
        window = glfwCreateWindow(WIDTH, HEIGHT, "3D-Engine", NULL, NULL);
 
    if (!window) {
        glfwTerminate();
        exit(-1);
    }
    glfwMakeContextCurrent(window);
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        exit(-1);
    }
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    if (WIREFRAME)
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
}

Window::~Window() {
    glfwTerminate();
}

bool Window::isShouldClose()
{
    return glfwWindowShouldClose(window);
}

void Window::swapBuffers()
{
    glfwSwapBuffers(window);
}

void Window::render()
{
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
}

void Window::setCursorMode(int mode)
{
    glfwSetInputMode(window, GLFW_CURSOR, mode);
}

void Window::framebuffer_size_callback(GLFWwindow *window, int width, int height) 
{
    glViewport(0, 0, width, height);
}