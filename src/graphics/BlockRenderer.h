#pragma once

#include "graphics/mesh.h"
#include "world/chunk.h"

class BlockRenderer {
    float* buffer;
    size_t capacity;
    public:
        BlockRenderer(size_t capacity);
        ~BlockRenderer();

        Mesh* render(Chunk* chunk);
};