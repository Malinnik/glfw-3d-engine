#pragma once

#include "graphics/LineBatch.h"
#include "graphics/shader.h"
#include "engine/camera/camera.h"

class BlockSelector{
    Camera* camera;
    Shader* shader;
    LineBatch* lineBatch;

    public:
        BlockSelector();
        ~BlockSelector();

        void draw(int x, int y, int z);
};