#include "gui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <algorithm>
#include <cstdlib>
#include <fstream>
#include <imgui.h>
#include <iterator>

imgui::imgui(GLFWwindow *window) { this->window = window; init();}

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

void imgui::loop()
{
  new_frame();
  main_bar();
  render();
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

void imgui::show_debug_shaders(const char *vshader, const char *fshader) {
  ImGui::Text("Vertex:");
  ImGui::Separator();
  ImGui::Text("%s", vshader);

  for (int i = 0; i < 10; i++)
    ImGui::Spacing();

  ImGui::Text("Fragment:");
  ImGui::Separator();
  ImGui::Text("%s", fshader);
}

void imgui::main_bar() {
  ImGui::BeginMainMenuBar();
  if (ImGui::BeginMenu("File", true)) {
    if (ImGui::MenuItem("Open")) {
    }
    if (ImGui::MenuItem("Save")) {
    }
    ImGui::EndMenu();
  }

  if (ImGui::BeginMenu("Debug", true)) {
    if (ImGui::Button("Show shader")) {
      ImGui::OpenPopup("Debug shader");
    }
    if (ImGui::Button("Show Metrics")) {
      ImGui::OpenPopup("Debug metrics");
    }
    if (ImGui::Button("Test File Save-Loading")) {
      ImGui::OpenPopup("Test File Save");
    }

    ImGui::Bullet();
    ImGui::Text("FPS: %f", ImGui::GetIO().Framerate);

    // if (ImGui::BeginPopup("Debug shader")) {
    //   // show_debug_shaders(vshader, fshader);
    //   ImGui::EndPopup();
    // }
    if (ImGui::BeginPopup("Debug metrics")) {
      ImGui::ShowMetricsWindow();
      ImGui::EndPopup();
    }
    if (ImGui::BeginPopup("Test File Save")) {
      struct Funcs {
        static int MyResizeCallback(ImGuiInputTextCallbackData *data) {
          if (data->EventFlag == ImGuiInputTextFlags_CallbackResize) {
            ImVector<char> *my_str = (ImVector<char> *)data->UserData;
            IM_ASSERT(my_str->begin() == data->Buf);
            my_str->resize(
                data->BufSize); // NB: On resizing calls, generally
                                // data->BufSize == data->BufTextLen + 1
            data->Buf = my_str->begin();
          }
          return 0;
        }
        static bool MyInputTextMultiline(const char *label,
                                         ImVector<char> *my_str,
                                         const ImVec2 &size = ImVec2(0, 0),
                                         ImGuiInputTextFlags flags = 0) {
          IM_ASSERT((flags & ImGuiInputTextFlags_CallbackResize) == 0);
          return ImGui::InputTextMultiline(
              label, my_str->begin(), (size_t)my_str->size(), size,
              flags | ImGuiInputTextFlags_CallbackResize,
              Funcs::MyResizeCallback, (void *)my_str);
        }

        static void SaveTextToFile(ImVector<char> *data) {
          std::ofstream output_file("./example.txt");
          std::ostream_iterator<char> output_iterator(output_file);
          std::copy(std::begin(*data), std::end(*data), output_iterator);
        }
      };

      static ImVector<char> str;
      if (str.empty())
        str.push_back(0);
      Funcs::MyInputTextMultiline(
          "##Save text to File", &str,
          ImVec2(-FLT_MIN, ImGui::GetTextLineHeight() * 16));
      ImGui::Text("Data: %p\nSize: %d\nCapacity: %d", (void *)str.begin(),
                  str.size(), str.capacity());
      if (ImGui::Button("Save"))
        Funcs::SaveTextToFile(&str);
      ImGui::EndPopup();
    }

    ImGui::EndMenu();
  }

  // Move button to the right
  ImGuiStyle &style = ImGui::GetStyle();
  float buttonWidth1 =
      ImGui::CalcTextSize("Exit").x + style.FramePadding.x * 2.f;
  float widthNeeded = buttonWidth1 + style.ItemSpacing.x;
  ImGui::SetCursorPosX(ImGui::GetCursorPosX() +
                       ImGui::GetContentRegionAvail().x - widthNeeded);
  if (ImGui::Button("Exit"))
    exit(0);

  ImGui::EndMainMenuBar();
}
