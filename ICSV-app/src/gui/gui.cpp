#include "gui.hpp"
#include "icsv/detector/arch/architecture_holder.hpp"
#include "icsv/detector/detector_manager.hpp"
#include "icsv/detector/evaluation_center.hpp"
#include <imgui/imgui.h>
#include "imguifilebrowser.hpp"

// Forward declarations
static void show_import_conf_gui(void);

static ImGui::FileBrowser file_dialog;

// Main show GUI function
void
show_menu(void) {
  if (ImGui::BeginMainMenuBar()) {
    show_import_conf_gui();
    ImGui::EndMainMenuBar();
  }
}

// Implementations
void
show_import_conf_gui(void) {
  if (ImGui::BeginMenu("File")) {
    if (ImGui::BeginMenu("Import")) {
      if (ImGui::MenuItem("Detector Config")) {
        file_dialog.SetTitle("Select Dectector Config");
        file_dialog.Open();
        if (file_dialog.HasSelected()) {
          DESERIALLIZE_DET_CONF(file_dialog.GetSelected());
          USE_DETECTORS;
        }
      }
      if (ImGui::MenuItem("Architecture Graph")) {
        file_dialog.SetTitle("Select Architecture Graph");
        file_dialog.Open();
        if (file_dialog.HasSelected()) {
          DESERIALLIZE_ARCH_CONF(file_dialog.GetSelected());
        }
      }
      ImGui::EndMenu();
    }
    ImGui::EndMenu();
  }
  file_dialog.Display();
}
