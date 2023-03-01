#include "gui.hpp"
#include "icsv/detector/arch/architecture_holder.hpp"
#include "icsv/detector/detector_manager.hpp"
#include "icsv/detector/evaluation_center.hpp"
#include "icsv/detector/report_center.hpp"
#include "../entity_manager.hpp"
#include "iostream"

namespace ICSVapp {

void
IcsvGui::Display(void) {
  ImGui::Begin("Menu");
  if (ImGui::CollapsingHeader("Smell Configuration")) {
    ShowConfigSelect();
    ShowSmellButton();
    ImGui::Separator();
  }
  if (ImGui::CollapsingHeader("Environment Settings")) {
    ShowSkyboxSettings();
    ImGui::Separator();
  }

  ShowDetectorReport();
  ImGui::End();
}

void
IcsvGui::ShowConfigSelect(void) {
  static std::string det_conf_file   = "";
  static std::string graph_conf_file = "";
  static char        graph_buf[200];
  static char        det_buf[200];

  ImGui::Text("Dectector Config File");
  ImGui::InputText("Selected Config", det_buf, 200);
  if (ImGui::Button("Select Config")) {
    m_conf_brwsr.SetTitle("Select Dectector Config");
    m_conf_brwsr.Open();
  }
  if (m_conf_brwsr.HasSelected()) {
    std::strncpy(det_buf, det_conf_file.c_str(), det_conf_file.size());
    det_conf_file = m_conf_brwsr.GetSelected();
    icsv::detector::deserialize_det_conf(m_conf_brwsr.GetSelected());
  }

  ImGui::Separator();

  ImGui::Text("Architecture Graph File");
  ImGui::InputText("Selected Graph", graph_buf, 200);
  if (ImGui::Button("Select Graph")) {
    m_graph_brwsr.SetTitle("Select Architecture Graph");
    m_graph_brwsr.Open();
  }
  if (m_graph_brwsr.HasSelected()) {
    std::strncpy(graph_buf, graph_conf_file.c_str(), graph_conf_file.size());
    graph_conf_file = m_graph_brwsr.GetSelected();
    icsv::detector::arch::deserialize_arch_conf(m_graph_brwsr.GetSelected());
  }
  m_conf_brwsr.Display();
  m_graph_brwsr.Display();
}

void
IcsvGui::ShowDetectorReport(void) {
  if (m_rep_dspld != nullptr) {
    ImGui::Begin("Detector Report");
    ImGui::MenuItem("Message: ");
    ImGui::SameLine();
    ImGui::MenuItem(m_rep_dspld->message.c_str());
    ImGui::Separator();

    ImGui::MenuItem("Level: ");
    ImGui::SameLine();
    ImGui::MenuItem(std::to_string(m_rep_dspld->level).c_str());
    ImGui::Separator();

    ImGui::MenuItem("File: ");
    ImGui::SameLine();
    ImGui::MenuItem(m_rep_dspld->src_info.file.c_str());
    ImGui::Separator();

    ImGui::MenuItem("Line: ");
    ImGui::SameLine();
    ImGui::MenuItem(std::to_string(m_rep_dspld->src_info.line).c_str());
    ImGui::Separator();

    ImGui::MenuItem("Column: ");
    ImGui::SameLine();
    ImGui::MenuItem(std::to_string(m_rep_dspld->src_info.col).c_str());
    ImGui::Separator();

    ImGui::MenuItem("Structure: ");
    ImGui::SameLine();
    ImGui::MenuItem(m_rep_dspld->src_info.strct.c_str());
    ImGui::Separator();

    ImGui::MenuItem("Method: ");
    ImGui::SameLine();
    ImGui::MenuItem(m_rep_dspld->src_info.method.c_str());
  }
  ImGui::End();
}

void
IcsvGui::ShowSmellButton(void) {
  if (ImGui::Button("Smell Code")) {
    icsv::detector::clear_reports();
    icsv::detector::use_smell_detectors();
    const auto& replst = icsv::detector::ReportCenter::Get().GetReportList();

    double x = 0, z = 0;
    auto   tag = replst.front()->smell_tag;
    for (auto* rep : replst) {
      if (tag != rep->smell_tag) {
        x++;
        z   = 0;
        tag = rep->smell_tag;
      }
      if (rep->level > 0) {
        IcsvEntity* ent = create_icsv_entity(rep);
        double      y   = 0.2 * rep->level;
        ent->SetScale(0.2, y, 0.2);
        ent->SetPosition(x, y / 2, z);
        z++;
      }
    }
  }
}

void
IcsvGui::ShowSkyboxSettings(void) {
  ImGui::Text("Skybox Color:");
  static bool   alpha        = true;
  static bool   alpha_bar    = true;
  static bool   side_preview = true;
  static int    display_mode = 0;
  static int    picker_mode  = 0;
  static ImVec4 color        = ImVec4(114.0f / 255.0f,
                               144.0f / 255.0f,
                               154.0f / 255.0f,
                               200.0f / 255.0f);

  ImGui::Checkbox("With Alpha", &alpha);
  ImGui::Checkbox("With Alpha Bar", &alpha_bar);
  ImGui::Checkbox("With Side Preview", &side_preview);

  ImGui::Combo("Display Mode",
               &display_mode,
               "Auto/Current\0None\0RGB Only\0HSV Only\0Hex Only\0");
  ImGuiColorEditFlags flags = 0;
  if (!alpha)
    flags |= ImGuiColorEditFlags_NoAlpha;  // This is by default if you call
                                           // ColorPicker3() instead of
                                           // ColorPicker4()
  if (alpha_bar)
    flags |= ImGuiColorEditFlags_AlphaBar;
  if (!side_preview)
    flags |= ImGuiColorEditFlags_NoSidePreview;
  if (picker_mode == 1)
    flags |= ImGuiColorEditFlags_PickerHueBar;
  if (picker_mode == 2)
    flags |= ImGuiColorEditFlags_PickerHueWheel;
  if (display_mode == 1)
    flags |= ImGuiColorEditFlags_NoInputs;  // Disable all RGB/HSV/Hex displays
  if (display_mode == 2)
    flags |= ImGuiColorEditFlags_DisplayRGB;  // Override display mode
  if (display_mode == 3)
    flags |= ImGuiColorEditFlags_DisplayHSV;
  if (display_mode == 4)
    flags |= ImGuiColorEditFlags_DisplayHex;
  ImGui::ColorPicker4("MyColor##4", (float*) &color, flags, NULL);
}

}  // namespace ICSVapp
