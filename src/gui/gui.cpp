#include "gui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <imgui.h>

#include <string>

imgui::imgui(GLFWwindow *window) { this->window = window; }

imgui::~imgui() { this->cleanup(); }

void imgui::init() {
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO &io = ImGui::GetIO();
  ImGui::StyleColorsDark();

  ImGui_ImplGlfw_InitForOpenGL(this->window, true);
  ImGui_ImplOpenGL3_Init();
}

void imgui::new_frame() {
  ImGui_ImplOpenGL3_NewFrame();
  ImGui_ImplGlfw_NewFrame();
  ImGui::NewFrame();
}

void imgui::render() {
  ImGui::Render();
  ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void imgui::cleanup() {
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();
}

float red = 0.1f;
float green = 0.0f;
ImVec2 button_size = ImVec2(100, 100);

void imgui::draw_frame() {
  ImGui::SetWindowPos(ImVec2(0, 0));
  ImGui::SetWindowSize(ImVec2(400, 400));

  ImGui::Begin("Demo window");

  ImGui::Text("FPS: %f", ImGui::GetIO().Framerate);

  ImGui::ColorEdit4("Test Color", &red);
  ImGui::Text("RED: %f", red);

  if (ImGui::Button("Add"))
    green += 0.1f;
  ImGui::SliderFloat("Test Slider", &green, 0.0f, 1.0f);
  ImGui::Text("Green: %f", green);

  ImGui::Text("Time: %f", ImGui::GetTime());

  if (ImGui::Button("Exit"))
    exit(0);

  ImGui::End();
}

void imgui::show_debug_shaders(const char *header, const char *vshader) {
  ImGui::SetWindowPos(ImVec2(0, 0));
  ImGui::SetWindowSize(ImVec2(400, 400));

  ImGui::Begin("Shader Debug");

  ImGui::Text(header);
  ImGui::Text(vshader);

  ImGui::End();
}
