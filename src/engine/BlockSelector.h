#pragma once

#include "graphics/LineBatch.h"
#include "graphics/shader.h"

class BlockSelector{
    Shader* shader;
    LineBatch* lineBatch;

    public:
        BlockSelector();
        ~BlockSelector();

        void draw(int x, int y, int z);
};