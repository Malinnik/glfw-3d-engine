#include "events.h"
#include <GLFW/glfw3.h>
#include <string.h>

#define MOUSE_BUTTONS 1024

bool* Events::keys;
uint* Events::frames;
uint Events::current = 0;
float Events::deltaX = 0.0f;
float Events::deltaY = 0.0f;
float Events::x = 0.0f;
float Events::y = 0.0f;
bool Events::cursor_locked = false;
bool Events::cursor_started = false;


void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    if (action = GLFW_PRESS)
    {
        Events::keys[key] = true;
        Events::frames[key] = Events::current;
    }
    else if (action = GLFW_RELEASE)
    {
        Events::keys[key] = false;
        Events::frames[key] = Events::current;
    }
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mode)
{
    if (action == GLFW_PRESS)
    {
        Events::keys[MOUSE_BUTTONS+button] = true;
        Events::frames[MOUSE_BUTTONS+button] = Events::current;
    }
    else if (action == GLFW_RELEASE)
    {
        Events::keys[MOUSE_BUTTONS+button] = false;
        Events::frames[MOUSE_BUTTONS+button] = Events::current;
    }
}

void cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
{
    if (Events::cursor_locked)
    {
        Events::deltaX += xpos-Events::x;
        Events::deltaY += xpos-Events::y;
    }
    else
    {
        Events::cursor_started = true;
    }
    Events::x = xpos;
    Events::y = ypos;
}

void Events::pullEvents()
{
    current++;
    deltaX = 0.0f;
    deltaY = 0.0f;
    glfwPollEvents();
}

Events::Events(GLFWwindow *window)
{
    keys = new bool[1032];
    frames = new uint[1032];

    memset(keys, false, 1032);
    memset(frames, 0, 1032);

    glfwSetKeyCallback(window, key_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    glfwSetCursorPosCallback(window, cursor_position_callback);
}

bool Events::pressed(int keyCode)
{
    if (keyCode < 0 || keyCode >= MOUSE_BUTTONS)
        return false;
    return keys[keyCode];
}

bool Events::jPressed(int keyCode)
{
    if (keyCode < 0 || keyCode >= MOUSE_BUTTONS)
        return false;
    return keys[keyCode] && frames[keyCode] == current;
}

bool Events::clicked(int button)
{
    int index = MOUSE_BUTTONS+button;
    return keys[index];
}

bool Events::jClicked(int button)
{
    int index = MOUSE_BUTTONS+button;
    return keys[index] && frames[index] == current;
}

void Events::toggleCursor()
{
    cursor_locked = !cursor_locked;
    Window::setCursorMode(cursor_locked ? GLFW_CURSOR_DISABLED : GLFW_CURSOR_NORMAL);
}
