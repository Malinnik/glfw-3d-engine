#pragma once
#include "GLFW/glfw3.h"


class App{
    public:
        App();
        ~App();
        GLFWwindow* getWindow();

    private:
        GLFWwindow *_window;
        void static framebuffer_size_callback(GLFWwindow *window, int width, int height);
};