#pragma once

#include <glm/glm.hpp>

using namespace glm;

class Chunk;
class block;

class Chunks {
    public:
        Chunk** chunks;
        size_t volume;

        unsigned int w,h,d;


        Chunks(int w, int h, int d);
        ~Chunks();

        block* get(int x, int y, int z);
        void set(int x, int y, int z, int id);
        block *rayCast(vec3 a, vec3 dir, float maxDist, vec3 &end, vec3 &norm, vec3 &iend);
        Chunk *getChunk(int x, int y, int z);
        
};