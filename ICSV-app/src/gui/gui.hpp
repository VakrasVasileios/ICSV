#pragma once

#include <imgui/imgui.h>
#include "imguifilebrowser.hpp"

namespace ICSVapp {

class IcsvGui final {
public:
  IcsvGui()  = default;
  ~IcsvGui() = default;

  void Display(void);

private:
  ImGui::FileBrowser m_graph_brwsr;
  ImGui::FileBrowser m_conf_brwsr;

  void ShowConfigSelect(void);
};

}  // namespace ICSVapp