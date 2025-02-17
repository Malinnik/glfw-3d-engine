#pragma once
// #include "GLFW/glfw3.h"

class GLFWwindow;

class Window{
    public:
        Window();
        ~Window();
        GLFWwindow* getWindow();
        bool isShouldClose();
        void swapBuffers();
        void render();

    private:
        GLFWwindow *_window;
        void static framebuffer_size_callback(GLFWwindow *window, int width, int height);
};