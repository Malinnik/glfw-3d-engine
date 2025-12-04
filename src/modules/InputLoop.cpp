#include "InputLoop.h"
#include "../config.h"
#include <loguru.hpp>

InputLoop::InputLoop(Camera* camera) : camera(camera){}

void InputLoop::updateTime(){
    float currentTime = glfwGetTime();
    delta = currentTime - lastTime;
    lastTime = currentTime;
}


void InputLoop::inputLoop(){
    updateTime();

    if (Events::jPressed(GLFW_KEY_TAB)){
        LOG_F(INFO, "Key TAB just pressed");
        Events::toggleCursor();
    }

    if (Events::pressed(GLFW_KEY_W)){
        LOG_F(INFO, "Key W pressed");
        camera->position += camera->front * delta * camera->speed;
    }
    if (Events::pressed(GLFW_KEY_S)){
        LOG_F(INFO, "Key S pressed");
        camera->position -= camera->front * delta * camera->speed;
    }
    if (Events::pressed(GLFW_KEY_D)){
        LOG_F(INFO, "Key D pressed");
        camera->position += camera->right * delta * camera->speed;
    }
    if (Events::pressed(GLFW_KEY_A)){
        LOG_F(INFO, "Key A pressed");
        camera->position -= camera->right * delta * camera->speed;
    }

    if (Events::pressed(GLFW_KEY_SPACE)){
        LOG_F(INFO, "Key SPACE pressed");
        camera->position += camera->up * delta * camera->speed;
    }

    if (Events::pressed(GLFW_KEY_LEFT_SHIFT)){
        LOG_F(INFO, "Key LEFT SHIFT pressed");
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