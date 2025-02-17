#include <glad/glad.h>

#include <GLFW/glfw3.h>

#include "config.h"
#include "gui.h"
#include "triangle.h"
#include "window.h"
#include "file.h"

int main(int argc, char *argv[]) {
  Window window;

  imgui gui(window.getWindow());
  gui.init();

  Triangle triangle; 

  while (!window.isShouldClose()) {
    glfwPollEvents();
    window.render();
    
    triangle.draw();
    gui.loop();

    window.swapBuffers();
  }
  
  return 0;
}
