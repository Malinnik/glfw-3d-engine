#include <glad/glad.h>

// #include <GLFW/glfw3.h>

#include "config.h"
#include "gui.h"
#include "triangle.h"
#include "window.h"
#include "file.h"
#include "events.h"
#include <iostream>

int main(int argc, char *argv[]) {
  if (LOGGING)
  {
    freopen("logs/output.log", "w", stdout);
    freopen("logs/error.log", "w", stderr);
  }
  Window window;
  Events events(window.getWindow());
  imgui gui(window.getWindow());
  Triangle triangle = Triangle("./assets/shaders/vertex.vert", "./assets/shaders/fragment.frag");
  // triangle.shader = Shader("./assets/shaders/vertex.vert", "./assets/shaders/fragment.frag");

  std::cout << "Startig program" << std::endl;

  while (!window.isShouldClose()) {
    events.pullEvents();
    window.render();
    
    triangle.draw();
    gui.loop();

    window.swapBuffers();
  }
  
  return 0;
}
