#pragma once
// #include "GLFW/glfw3.h"

class GLFWwindow;

class Window{
    public:
        Window();
        ~Window();
        bool isShouldClose();
        void swapBuffers();
        void render();
        
        static void setCursorMode(int mode);
        static GLFWwindow *window;

    private:
        void static framebuffer_size_callback(GLFWwindow *window, int width, int height);
};