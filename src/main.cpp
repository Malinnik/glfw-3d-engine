#include <glad/glad.h>

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
  Triangle triangle = Triangle("./assets/shaders/vertex.vert", "./assets/shaders/fragment.frag");
  Camera* camera = new Camera(vec3(0,0,1), radians(90.f));

  InputLoop inputLoop = InputLoop(camera);

  while (!window.isShouldClose()) {
    window.render();
    
    triangle.draw();
    gui.loop();
    
    inputLoop.inputLoop();
    
    window.swapBuffers();
    events.pullEvents();
  }
  
  return 0;
}
