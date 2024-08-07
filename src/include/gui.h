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
  void show_debug_shaders(const char *header, const char *vshader);

  void render();
  void cleanup();
};
