#include "world.h"
#include "loaders/image/png_loader.h"
#include "glm/glm.hpp"
#include "glm/ext.hpp"

World::World()
{
    shader = new Shader("./assets/shaders/block.vert", "./assets/shaders/block.frag");
    texture = load_texture("./assets/images/TextureAtlas.png");
    camera = new Camera(vec3(20,15,20), radians(70.0f));
    crosshair = new Crosshair();
    
    chunks = new Chunks(8,4,8);
    meshes = new Mesh*[chunks->volume];
    for (size_t i = 0; i < chunks->volume; i++)
        meshes[i] = nullptr;

    inputLoop = new InputLoop(camera, chunks);
    
}

World::~World()
{
    delete shader;
    delete texture;
    delete camera;
    delete crosshair;
    delete chunks;
    delete meshes;
    delete inputLoop;
}

void World::draw()
{
    inputLoop->inputLoop();

    reRenderChunks();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    shader->use();
    texture->bind();
    shader->uniformMatrix("projview", camera->getProjection()*camera->getView());
    mat4 model(1.0f);
    for (size_t i = 0; i < chunks->volume; i++){
        Chunk* chunk = chunks->chunks[i];
        Mesh* mesh = meshes[i];
        model = glm::translate(mat4(1.0f), vec3(chunk->x*CHUNK_W+0.5f, chunk->y*CHUNK_H+0.5f, chunk->z*CHUNK_D+0.5f));
        shader->uniformMatrix("model", model);
        mesh->draw(GL_TRIANGLES);
    }

    crosshair->draw();

}


void World::reRenderChunks(){
    Chunk* closes[27];
    for (size_t i = 0; i < chunks->volume; i++){
        Chunk* chunk = chunks->chunks[i];
        if (!chunk->modified)
            continue;
        chunk->modified = false;
        if (meshes[i] != nullptr)
            delete meshes[i];

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
        meshes[i] = mesh;
    }
}