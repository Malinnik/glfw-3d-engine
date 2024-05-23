#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "gui.h"

int main(int argc, char *argv[]) {

  GLFWwindow *window;

  if (!glfwInit())
    return -1;

  window = glfwCreateWindow(640, 480, "3D-Engine", NULL, NULL);
  if (!window) {
    glfwTerminate();
    return -1;
  }

  glfwMakeContextCurrent(window);



  imgui gui(window);
  gui.init();

  while (!glfwWindowShouldClose(window)) {
    // ImGui frames
    gui.new_frame();
    gui.draw_frame();

    // GLFW render
    glClear(GL_COLOR_BUFFER_BIT);

    // ImGui render
    gui.render();

    // Swap front and back buffers 
    glfwSwapBuffers(window);

    // Poll for and process events 
    glfwPollEvents();
  }


  // gui.cleanup();
  glfwTerminate();

  return 0;
}
