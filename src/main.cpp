#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <iostream>
#include <loguru.hpp>

// #include <GLFW/glfw3.h>

#include "loaders/image/png_loader.h"
#include "graphics/triangle.h"
#include "graphics/quad.h"
#include "graphics/texture.h"
#include "graphics/BlockRenderer.h"
#include "engine/InputLoop.h"
#include "engine/camera.h"
#include "engine/crosshair.h"
#include "window/window.h"
#include "window/events.h"
#include "world/chunk.h"
#include "world/chunks.h"
#include "files/file.h"
#include "config.h"
#include "ui/gui.h"

int main(int argc, char *argv[]) {

  loguru::init(argc, argv);
  loguru::add_file("logs/debug.log", loguru::Append, loguru::Verbosity_MAX);
  loguru::add_file("logs/info.log", loguru::Truncate, loguru::Verbosity_INFO);
  loguru::add_file("logs/error.log", loguru::Truncate, loguru::Verbosity_ERROR);

  LOG_F(INFO, "Starting program");

  Window window = Window();
  Events events(Window::window);
  imgui gui(Window::window);

  Shader *shader = new Shader("./assets/shaders/texture_light.vert", "./assets/shaders/texture_fragment.frag");
  Texture* texture = load_texture("./assets/images/TextureAtlas.png");
  Camera* camera = new Camera(vec3(0,0,20), radians(70.0f));
  Crosshair crosshair = Crosshair();
  
  Chunks* chunks = new Chunks(32,4,32);
  Mesh** meshes = new Mesh*[chunks->volume];
  for (size_t i = 0; i < chunks->volume; i++)
    meshes[i] = nullptr;
  BlockRenderer blockRenderer(1024*1024*8);
  
  glClearColor(0.6f,0.62f,0.65f,1);
  glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  
  
  InputLoop inputLoop = InputLoop(camera, chunks);
  
  while (!window.isShouldClose()) {
    inputLoop.inputLoop();
    window.render();
    
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

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    shader->use();
		shader->uniformMatrix("projview", camera->getProjection()*camera->getView());
    texture->bind();

    mat4 model(1.0f);
		for (size_t i = 0; i < chunks->volume; i++){
			Chunk* chunk = chunks->chunks[i];
			Mesh* mesh = meshes[i];
			model = glm::translate(mat4(1.0f), vec3(chunk->x*CHUNK_W+0.5f, chunk->y*CHUNK_H+0.5f, chunk->z*CHUNK_D+0.5f));
			shader->uniformMatrix("model", model);
			mesh->draw(GL_TRIANGLES);
		}
    
    crosshair.draw();

    gui.loop();
    window.swapBuffers();
    events.pullEvents();
  }
  
  delete shader;
  delete camera;
  delete chunks;

  return 0;
}
