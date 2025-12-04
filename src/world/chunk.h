#pragma once

#include "blocks/block.h"

#define CHUNK_W 16
#define CHUNK_H 16
#define CHUNK_D 16
#define CHUNK_BLOCKS (CHUNK_W * CHUNK_H * CHUNK_D)

// [(Y*D+Z) * W + X]

class Chunk{
    public:
        int x, y, z;
        block* blocks;
        bool modified = true;
        Chunk(int x, int y, int z);
        ~Chunk();
};