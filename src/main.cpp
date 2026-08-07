#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <iostream>
#include <loguru.hpp>

#include <GLFW/glfw3.h>

#include "window/window.h"
#include "window/events.h"
#include "world/world.h"
#include "world/WorldManager.h"
#include "ui/gui.h"

int main(int argc, char *argv[]) {

  loguru::init(argc, argv);
  loguru::add_file("logs/debug.log", loguru::Truncate, loguru::Verbosity_MAX);
  loguru::add_file("logs/info.log", loguru::Truncate, loguru::Verbosity_INFO);
  loguru::add_file("logs/error.log", loguru::Truncate, loguru::Verbosity_ERROR);

  LOG_F(INFO, "Starting program");

  Window window = Window();
  Events events(Window::window);
  imgui gui(Window::window);

  auto& worldManager = WorldManager::instace();
  worldManager.createWorld<World>();
  
  glClearColor(0.6f,0.62f,0.65f,1);
  glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  
  LOG_F(INFO, "Starting world draw cycle");
  while (!window.isShouldClose()) {
    window.render();
    
    worldManager.getActiveWorld()->draw();

    gui.loop();
    window.swapBuffers();
    events.pullEvents();
  }
  LOG_F(INFO, "Detected app closing");
  worldManager.gracefulShutdown();
  return 0;
}
