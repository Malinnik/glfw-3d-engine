#pragma once

#include <GLFW/glfw3.h>

#include "engine/camera.h"
#include "window/events.h"
#include "world/chunks.h"


class InputLoop{
    Camera* camera;
    Chunks* chunks;
    
    private:
        void updateTime();
        
    public:
        void inputLoop();
        float lastTime = glfwGetTime();
        float delta = 0.0f;
        InputLoop(Camera* camera, Chunks* chunks);
};