#include "gui.hpp"
#include "icsv/detector/arch/architecture_holder.hpp"
#include "icsv/detector/detector_manager.hpp"
#include "icsv/detector/evaluation_center.hpp"
#include "iostream"

namespace ICSVapp {

void
IcsvGui::Display(void) {
  ImGui::Begin("Menu");
  ShowConfigSelect();
  ImGui::Separator();
  ImGui::Text("Smell Report");
  // check if something was clicked by Raycast
  // and Display Report
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

}  // namespace ICSVapp
