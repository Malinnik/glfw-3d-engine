#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "gui.h"

#include <iostream>

#define WIDTH 1920
#define HEIGHT 1080


// change viewport size after window size change
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
} 

int main(int argc, char *argv[]) {

  GLFWwindow *window;

  if (!glfwInit())
    return -1;

  window = glfwCreateWindow(WIDTH, HEIGHT, "3D-Engine", NULL, NULL);
  if (!window) {
    glfwTerminate();
    return -1;
  }
  glfwMakeContextCurrent(window);


  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
  {
      std::cout << "Failed to initialize GLAD" << std::endl;
      return -1;
  } 
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback); 


  imgui gui(window);
  gui.init();

  while (!glfwWindowShouldClose(window)) {
    // ImGui frames
    gui.new_frame();
    gui.draw_frame();

    // GLFW render
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
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
