#include "world.h"
#include "loaders/image/png_loader.h"
#include <glad/glad.h>
#include "glm/glm.hpp"
#include "glm/ext.hpp"
#include <loguru.hpp>


World::World()
{
    blocks::initializeBlocks();
    Atlas::build();

    shader = new Shader("./assets/shaders/block.vert", "./assets/shaders/block.frag");
    texture = Atlas::textureAtlas;
    int playerY = WorldGenerator::getTerrainHeight(20, 20);

    player = std::make_unique<entity::player::Player>(Transform(0, playerY, 0));
    chunks = std::make_unique<Chunks>(16*2,8,16*2, 0,0,0);

    CameraManager::instance().pushCamera(player->getCamera());
    // CameraManager::instance().pushCamera(player.get)

}

World::~World()
{
    delete shader;
    delete texture;
}

void World::update(float delta)
{
    if (player)
        player->onUpdate(delta);
}

void World::draw()
{
    Camera* camera = CameraManager::instance().getActiveCamera();
    if (!camera) return;
    camera->updateFrustum();

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    
    chunks->setCenter(camera->position.x, camera->position.y, camera->position.z);
    chunks->_buildMeshes(&blockRenderer);
    chunks->loadVisible(&worldFiles);

    shader->use();
    // ensure texture unit 0 active and sampler points to it
    glActiveTexture(GL_TEXTURE0);
    texture->bind();
    shader->uniformInt("u_texture0", 0);
    shader->uniformMatrix("projview", camera->getProjection()*camera->getView());
    mat4 model(1.0f);
    // player->onRender();
    for (size_t i = 0; i < chunks->volume; i++){
        Chunk* chunk = chunks->chunks[i];
        if (chunk == nullptr)
            continue;
        Mesh* mesh = chunks->meshes[i];
        if (mesh == nullptr)
            continue;

        //Отсечение невидимых чанков
        glm::vec3 chunkMin = glm::vec3(chunk->x * CHUNK_W, chunk->y * CHUNK_H, chunk->z * CHUNK_D);
        glm::vec3 chunkMax = chunkMin + glm::vec3(CHUNK_W, CHUNK_H, CHUNK_D);
        if (!camera->frustum.isAABBVisible(chunkMin, chunkMax)) {
            continue;
        }

        model = glm::translate(mat4(1.0f), vec3(chunk->x*CHUNK_W+0.5f, chunk->y*CHUNK_H+0.5f, chunk->z*CHUNK_D+0.5f));
        shader->uniformMatrix("model", model);
        mesh->draw(GL_TRIANGLES);
    }

    player->onRender();

}

void World::save(){
    LOG_F(INFO, "Saving world...");
    for (unsigned int i = 0; i < chunks->volume; i++){
		Chunk* chunk = chunks->chunks[i];
		if (chunk == nullptr)
			continue;
        worldFiles.put((const char*)chunk->blocksIds, chunk->x, chunk->y, chunk->z);
	}
    worldFiles.write();
}


void World::reRenderChunks(){
    Chunk* closes[27];
    for (size_t i = 0; i < chunks->volume; i++){
        Chunk* chunk = chunks->chunks[i];
        if (!chunk->modified)
            continue;
        chunk->modified = false;
        if (chunks->meshes[i] != nullptr)
            delete chunks->meshes[i];

        for (int i = 0; i < 27; i++)
            closes[i] = nullptr;
        for (size_t j = 0; j < chunks->volume; j++){
            Chunk* other = chunks->chunks[j];

            int ox = other->x - chunk->x;
            int oy = other->y - chunk->y;
            int oz = other->z - chunk->z;

            if (abs(ox) > 1 || abs(oy) > 1 || abs(oz) > 1)
                continue;

            ox += 1;
            oy += 1;
            oz += 1;
            closes[(oy * 3 + oz) * 3 + ox] = other;
        }
        Mesh* mesh = blockRenderer.render(chunk, (const Chunk**)closes);
        chunks->meshes[i] = mesh;
    }
}