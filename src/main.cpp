#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>

// #include <GLFW/glfw3.h>

#include "config.h"
#include "gui.h"
#include "triangle.h"
#include "window.h"
#include "file.h"
#include "events.h"
#include <iostream>
#include <loguru.hpp>
#include "camera.h"
#include "loaders/image/png_loader.h"
#include "texture.h"
#include "InputLoop.h"

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
  Triangle triangle = Triangle(shader);
  
  Camera* camera = new Camera(vec3(0,0,1), radians(70.0f));
  InputLoop inputLoop = InputLoop(camera);
  
  texture->bind();
  
  while (!window.isShouldClose()) {
    inputLoop.inputLoop();
    window.render();
    
    triangle.draw(camera);
    gui.loop();
    
    window.swapBuffers();
    events.pullEvents();
  }
  
  delete shader;
  delete camera;
  return 0;
}
