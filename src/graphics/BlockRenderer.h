#pragma once

#include "graphics/mesh.h"
#include "blocks/block.h"
#include "world/chunk.h"

class BlockRenderer {
    float* buffer;
    size_t capacity;
    Chunk** currentChunks;

    public:
        BlockRenderer(size_t capacity);
        ~BlockRenderer();

        std::tuple<float, float, float, float> calculateUV(resources::ResourceLocation *location);

        Mesh* render(Chunk* chunk, const Chunk** chunks, bool ambientOcclusion = true);

    // private:
        // int cdiv(int x, int a);
        // int localNeg(int x, int size);
        // int local(int x, int size);
        // const Chunk* getChunk(int x, int y, int z, const Chunk** chunks);
        // bool isChunk(int x, int y, int z, const Chunk** chunks);

        // blocks::Block* block(int x, int y, int z, const Chunk** chunks);        
        // bool isBlocked(int x, int y, int z, const Chunk** chunks);
        // void vertex(size_t* index, float x, float y, float z, float u, float v, float l);


};