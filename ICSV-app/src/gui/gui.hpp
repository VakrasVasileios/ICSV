#pragma once

#include <imgui/imgui.h>
#include "imguifilebrowser.hpp"
#include "icsv/detector/detector_report.hpp"

namespace ICSVapp {

using DetectorReport = icsv::detector::DetectorReport;

struct Selectable;

class IcsvGui final {
public:
  static auto Get() -> IcsvGui&;

  void Display(void);
  void SetReportToDisplay(DetectorReport* rep) { m_rep_dspld = rep; }
  void SetSkyboxColorPtr(float* sb_color) { m_skybox_color = sb_color; }
  void SetCameraData(float* speed, float* rot_smooth) {
    m_cam_data = CameraGuiData(speed, rot_smooth);
  }

private:
  ImGui::FileBrowser              m_graph_brwsr;
  ImGui::FileBrowser              m_conf_brwsr;
  icsv::detector::DetectorReport* m_rep_dspld{ nullptr };
  float*                          m_skybox_color{ nullptr };
  struct CameraGuiData {
    float* cam_speed{ nullptr };
    float* rot_smooth{ nullptr };
    CameraGuiData() = default;
    CameraGuiData(float* _s, float* _r) : cam_speed(_s), rot_smooth(_r) {}
  } m_cam_data;

  IcsvGui()               = default;
  IcsvGui(const IcsvGui&) = delete;
  IcsvGui(IcsvGui&&)      = delete;
  ~IcsvGui()              = default;

  void ShowConfigSelect(void);
  void ShowDetectorReport(void);
  void ShowSmellButton(bool& changed);
  void ShowSkyboxSettings(void);
  void ShowCameraSettings(void);
  void ShowSortingSettings(void);
  void ShowSpecialFX(void);
  void ShowEvalConfigs(void);

  void SortEntityList(const std::string& by_tag);
};

struct Selectable {
  Selectable(const std::string& tag = "") : m_tag(tag) {}
  std::string m_tag;
  bool        m_sel = false;
};

}  // namespace ICSVapp