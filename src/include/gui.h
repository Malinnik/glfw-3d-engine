#pragma once 
#include <GLFW/glfw3.h>

class imgui
{
    private:
        GLFWwindow *window;
    public:
        imgui(GLFWwindow *window);
        ~imgui();

        void init();
        void new_frame();

        void draw_frame();

        void render();
        void cleanup();

};


