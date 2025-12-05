#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <iostream>
#include <loguru.hpp>

#include <GLFW/glfw3.h>

#include "window/window.h"
#include "window/events.h"
#include "world/world.h"
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

  World* world = new World();

  glClearColor(0.6f,0.62f,0.65f,1);
  glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  
  while (!window.isShouldClose()) {
    window.render();
    
    world->draw();

    gui.loop();
    window.swapBuffers();
    events.pullEvents();
  }

  return 0;
}
