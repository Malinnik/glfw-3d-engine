#include "InputLoop.h"

InputLoop::InputLoop(Camera* camera) : camera(camera){}

void InputLoop::updateTime(){
    float currentTime = glfwGetTime();
    delta = currentTime - lastTime;
    lastTime = currentTime;
}


void InputLoop::inputLoop(){
    updateTime();
}