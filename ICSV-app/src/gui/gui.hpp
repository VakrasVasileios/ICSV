#pragma once

#include <imgui/imgui.h>
#include "imguifilebrowser.hpp"
#include "icsv/detector/detector_report.hpp"

namespace ICSVapp {

using DetectorReport = icsv::detector::DetectorReport;

class IcsvGui final {
public:
  IcsvGui()  = default;
  ~IcsvGui() = default;

  void Display(void);
  void SetReportToDisplay(DetectorReport* rep) { m_rep_dspld = rep; }
  void SetSkyboxColorPtr(float* sb_color) { m_skybox_color = sb_color; }

private:
  ImGui::FileBrowser              m_graph_brwsr;
  ImGui::FileBrowser              m_conf_brwsr;
  icsv::detector::DetectorReport* m_rep_dspld{ nullptr };
  float*                          m_skybox_color{ nullptr };

  void ShowConfigSelect(void);
  void ShowDetectorReport(void);
  void ShowSmellButton(void);
  void ShowSkyboxSettings(void);
};

}  // namespace ICSVapp