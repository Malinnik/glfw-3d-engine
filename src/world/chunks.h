#pragma once

#include <glm/glm.hpp>
#include <vector>

#include "blocks/block.h"
#include "graphics/BlockRenderer.h"
#include "world/ChunkGenerator.h"
#include "files/WorldFiles.h"

using namespace glm;

class Chunk;

class Chunks {
    public:
        Chunk **chunks;
        Chunk **chunksSecond;
        Mesh **meshes;
        Mesh **meshesSecond;
        size_t volume;

        ChunkGenerator* generator = nullptr;
        std::vector<ChunkResult> pendingChunks;

        unsigned int w,h,d;
        int ox, oy, oz;

        Chunks(int w, int h, int d, int ox, int oy, int oz);
        ~Chunks();

        Chunk* getChunk(int x, int y, int z);
        int getChunkIndex(int x, int y, int z);
        Chunk* getChunkByBlock(int x, int y, int z);

        blocks::Block* get(int x, int y, int z);
        blocks::Block *rayCast(vec3 a, vec3 dir, float maxDist, vec3 &end, vec3 &norm, vec3 &iend);

        void set(int x, int y, int z, int id);

        void setCenter(int x, int y, int z);
        void translate(int dx, int dy, int dz);

        bool loadVisible(WorldFiles *worldFiles);
        bool _buildMeshes(BlockRenderer *renderer);

        void setGenerator(ChunkGenerator* generator) {this->generator = generator;}
        void update();
};