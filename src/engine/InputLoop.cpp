#include <loguru.hpp>

#include "config.h"
#include "InputLoop.h"

InputLoop::InputLoop(Camera* camera) : camera(camera){}

void InputLoop::updateTime(){
    float currentTime = glfwGetTime();
    delta = currentTime - lastTime;
    lastTime = currentTime;
}


void InputLoop::inputLoop(){
    updateTime();

    if (Events::jPressed(GLFW_KEY_TAB)){
        Events::toggleCursor();
    }

    if (Events::pressed(GLFW_KEY_W)){
        camera->position += camera->front * delta * camera->speed;
    }
    if (Events::pressed(GLFW_KEY_S)){
        camera->position -= camera->front * delta * camera->speed;
    }
    if (Events::pressed(GLFW_KEY_D)){
        camera->position += camera->right * delta * camera->speed;
    }
    if (Events::pressed(GLFW_KEY_A)){
        camera->position -= camera->right * delta * camera->speed;
    }

    if (Events::pressed(GLFW_KEY_SPACE)){
        camera->position += camera->up * delta * camera->speed;
    }

    if (Events::pressed(GLFW_KEY_LEFT_SHIFT)){
        camera->position -= camera->up * delta * camera->speed;
    }

    if (Events::cursor_locked){
        camera->camY += -Events::deltaY / HEIGHT * 2;
        camera->camX += -Events::deltaX / HEIGHT * 2;

        if (camera->camY < -radians(89.0f))
            camera->camY = -radians(89.0f);
        if (camera->camY > radians(89.0f))
            camera->camY = radians(89.0f);

        camera->rotation = mat4(1.0f);
        camera->rotate(camera->camY, camera->camX, 0);
    }
}