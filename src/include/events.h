#pragma once
#include <GLFW/glfw3.h>
#include <window.h>

typedef unsigned int uint;
// class GLFWwindow;

class Events {
    public:
        static bool* keys;
        static uint* frames;
        static uint current;
        static float deltaX;
        static float deltaY;
        static float x;
        static float y;
        static bool cursor_locked;
        static bool cursor_started;
        
        void pullEvents();
        Events(GLFWwindow *window);
        // ~Events();

        static bool pressed(int keyCode);
        static bool jPressed(int keyCode);

        static bool clicked(int button);
        static bool jClicked(int button);

        static void toggleCursor();
};