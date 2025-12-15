#pragma once

#include "graphics/shader.h"
#include "graphics/texture.h"
#include "graphics/mesh.h"
#include "graphics/BlockRenderer.h"
#include "engine/camera.h"
#include "engine/crosshair.h"
#include "engine/InputLoop.h"
#include "world/chunks.h"

class World{
    Shader* shader;
    Texture* texture;
    Camera* camera;
    Crosshair* crosshair;

    Chunks* chunks;
    Mesh** meshes;

    InputLoop* inputLoop;
    BlockRenderer blockRenderer = BlockRenderer(size_t(1024*1024*8));

    public:
        World();
        ~World();

        void draw();
        void reRenderChunks();
};