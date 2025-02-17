#include <glad/glad.h>

#include <GLFW/glfw3.h>

#include "config.h"
#include "gui.h"
#include "triangle.h"
#include "app.h"

int main(int argc, char *argv[]) {
  App app;

  imgui gui(app.getWindow());
  gui.init();

  Triangle triangle;

  while (!glfwWindowShouldClose(app.getWindow())) {
    // ImGui frames
    gui.new_frame();
    gui.main_bar("", "");

    // Poll for and process events
    glfwPollEvents();

    // GLFW render
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    triangle.draw();
    
    // ImGui render
    gui.render();
    // Swap front and back buffers
    glfwSwapBuffers(app.getWindow());
  }
  
  return 0;
}
