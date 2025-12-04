#pragma once

#include <GLFW/glfw3.h>

#include "engine/camera.h"
#include "window/events.h"



class InputLoop{
    Camera* camera;
    
    private:
        void updateTime();
        
    public:
        void inputLoop();
        float lastTime = glfwGetTime();
        float delta = 0.0f;
        InputLoop(Camera* camera);
};