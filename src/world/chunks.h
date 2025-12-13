#pragma once

#include <glm/glm.hpp>
#include "blocks/block.h"

using namespace glm;

class Chunk;

class Chunks {
    public:
        Chunk** chunks;
        size_t volume;

        unsigned int w,h,d;


        Chunks(int w, int h, int d);
        ~Chunks();

        blocks::Block* get(int x, int y, int z);
        void set(int x, int y, int z, int id);
        blocks::Block *rayCast(vec3 a, vec3 dir, float maxDist, vec3 &end, vec3 &norm, vec3 &iend);
        void write(unsigned char *path);
        void read(unsigned char *path);
        Chunk *getChunk(int x, int y, int z);
};