#pragma once
#include <GLFW/glfw3.h>

class imgui {
private:
  GLFWwindow *window;

public:
  imgui(GLFWwindow *window);
  ~imgui();

  void init();
  void new_frame();

  void draw_frame();
  void show_debug_shaders(const char *vshader, const char *fshader);

  void main_bar();

  void ShowPerlinNoisePopup(const char *popupName);

  void render();
  void cleanup();

  void loop();
};
