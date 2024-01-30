#pragma once

#include <imgui/imgui.h>
#include "imguifilebrowser.hpp"
#include "icsv/detector/detector_report.hpp"
#include <functional>

namespace ICSVapp {

using DetectorReport = icsv::detector::DetectorReport;
using ReportMembers  = icsv::detector::ReportMembers;

struct Selectable;

class IcsvGui final {
public:
  using ExitCall = std::function<void(void)>;

public:
  static auto Get() -> IcsvGui&;

  void Display(void);
  void SetReportToDisplay(DetectorReport* rep) { m_rep_dspld = rep; }
  void SetSkyboxColorPtr(float* sb_color) { m_skybox_color = sb_color; }
  void SetCameraData(float* speed, float* rot_smooth) {
    m_cam_data = CameraGuiData(speed, rot_smooth);
  }
  void SetExitCallBack(const ExitCall& ec) { m_exit_call = ec; }
  void SetRenderExitPopUp(bool pop) { m_render_exit_pop = pop; }

private:
  ImGui::FileBrowser m_graph_brwsr;
  ImGui::FileBrowser m_conf_brwsr;
  DetectorReport*    m_rep_dspld{ nullptr };
  float*             m_skybox_color{ nullptr };
  ExitCall           m_exit_call;
  bool               m_render_exit_pop;

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
  void ShowActiveDetectors(void);
  void ShowDetectorReport(void);
  void ShowEvalConfigs(void);
  void ShowExitPopUp(void);
  void ShowExportTab(void);
  void ShowSmellButton(bool& changed);
  void ShowSkyboxSettings(void);
  void ShowCameraSettings(void);
  void ShowSortingSettings(void);
  void ShowSmellColorPallet(void);

  void SortEntityList(ReportMembers x_axis, ReportMembers z_axis);
};

struct Selectable {
  Selectable(const std::string& tag  = "",
             ReportMembers      memb = ReportMembers::EMPTY_m)
      : m_tag(tag), m_memb(memb) {}
  std::string   m_tag;
  ReportMembers m_memb;
  bool          m_sel = false;
};

}  // namespace ICSVapp