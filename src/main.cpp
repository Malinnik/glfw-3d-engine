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

  window = glfwCreateWindow(WIDTH, HEIGHT, "3D-Engine", glfwGetPrimaryMonitor(), NULL);
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


  // Triangle

  GLfloat vertices[] = {
    -0.5f, -0.5f, 0.0f,
     0.5f, -0.5f, 0.0f,
     0.0f,  0.5f, 0.0f
  };

  // float vertices[] = {
  //   0.5f,  0.5f, 0.0f,  // top right
  //   0.5f, -0.5f, 0.0f,  // bottom right
  //   -0.5f, -0.5, 0.0f,  // bottom left
  //   -0.5f,  0.5f, 0.0f   // top left  
  // };

  unsigned int indices[] = {
    0,1,3,
    1,2,3
  };

  GLuint VAO, VBO, EBO;
  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);
  glGenBuffers(1, &EBO);

  (VAO);

  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);


  while (!glfwWindowShouldClose(window)) {
    // ImGui frames
    gui.new_frame();
    gui.draw_frame();

    // GLFW render
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    // ImGui render
    gui.render();

    // Swap front and back buffers 
    glfwSwapBuffers(window);

    // Poll for and process events 
    glfwPollEvents();
  }

  glDeleteVertexArrays(1, &VAO);
  glDeleteBuffers(1, &VBO);
  glDeleteBuffers(1, &EBO);

  // gui.cleanup();
  glfwTerminate();

  return 0;
}
