#pragma once

#include "graphics/LineBatch.h"
#include "graphics/shader.h"
#include "engine/camera.h"

class BlockSelector{
    Camera* camera;
    Shader* shader;
    LineBatch* lineBatch;

    public:
        BlockSelector(Camera* camera);
        ~BlockSelector();

        void draw(int x, int y, int z);
};