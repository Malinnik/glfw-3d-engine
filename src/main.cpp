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
#include "engine/InputLoop.h"
#include "window/window.h"
#include "window/events.h"
#include "engine/camera.h"
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

  // Shader *shader = new Shader("./assets/shaders/vertex.vert", "./assets/shaders/fragment.frag");
  Shader *shader = new Shader("./assets/shaders/texture_vertex.vert", "./assets/shaders/texture_fragment.frag");
  Texture* texture = load_texture("./assets/images/dirt.png");
  Quad quad = Quad(shader);
  texture->bind();
  
  Camera* camera = new Camera(vec3(0,0,1), radians(70.0f));
  InputLoop inputLoop = InputLoop(camera);
  
  while (!window.isShouldClose()) {
    inputLoop.inputLoop();
    window.render();
    
    quad.draw(camera);
    gui.loop();
    
    window.swapBuffers();
    events.pullEvents();
  }
  
  delete shader;
  delete camera;
  return 0;
}
