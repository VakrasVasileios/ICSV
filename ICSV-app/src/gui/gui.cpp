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
  ShowConfigSelect();
  ShowSmellButton();
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
        IcsvEntity* ent = create_icsv_entity("ogrehead.mesh", rep);
        double      y   = 0.1 * rep->level;
        ent->SetScale(0.1, y, 0.1);
        ent->SetPosition(x, y / 2, z);
        z++;
      }
    }
  }
}

}  // namespace ICSVapp
