#pragma once

#include <memory>

#include "graphics/shader.h"
#include "graphics/texture.h"
#include "graphics/mesh.h"
#include "graphics/BlockRenderer.h"
#include "world/chunks.h"
#include "entities/player/player.h"
#include "engine/camera/CameraManager.h"
#include "blocks/blocks.h"
#include "graphics/atlas.h"
#include "world/generation.h"
#include "files/WorldFiles.h"


class World{
    Shader* shader;
    Texture* texture;

    BlockRenderer blockRenderer = BlockRenderer(size_t(1024*1024*8));
    WorldFiles worldFiles = WorldFiles("./world/overworld/", size_t(24*1024*1024));


    public:
        World();
        ~World();

        void update(float delta);
        void draw();
        void save();
        void reRenderChunks();

        Chunks* getChunks() { return chunks.get(); }
        entity::player::Player* getPlayer() { return player.get(); }

        const float Gravity = 9.8;
    
    private:
        std::unique_ptr<Chunks> chunks;
        std::unique_ptr<entity::player::Player> player;
};