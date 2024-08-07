#pragma once
#include <GLFW/glfw3.h>
#include <string>

class imgui {
private:
  GLFWwindow *window;

public:
  imgui(GLFWwindow *window);
  ~imgui();

  void init();
  void new_frame();

  void draw_frame();
  void show_debug_shaders(char *header, const GLchar *vshader);

  void render();
  void cleanup();
};
