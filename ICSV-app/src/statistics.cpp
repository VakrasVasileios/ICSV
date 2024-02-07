#include "statistics.hpp"
#include <imgui/imgui.h>

namespace ICSVapp {

auto
SmellStatistics::Get(void) -> SmellStatistics& {
  static SmellStatistics singleton;
  return singleton;
}

void
SmellStatistics::AddSmellIntensity(const std::string& tag, float lvl) {
  if (!m_smell_int_map.contains(tag)) {
    SmellIntesity si;

    si.level   = lvl;
    si.average = lvl;
    si.count   = 1;
    int indx   = static_cast<int>(lvl);
    si.level_c[(indx == 10) ? 9 : indx]++;

    m_smell_int_map.insert({ tag, si });
  } else {
    auto& s = m_smell_int_map.at(tag);
    s.level += lvl;
    s.count++;
    s.average = s.level / s.count;
    int indx  = static_cast<int>(lvl);
    s.level_c[(indx == 10) ? 9 : indx]++;
  }
  m_overall_int += lvl;
}

void
SmellStatistics::AddFileIntensity(const std::string& file, float lvl) {
  if (!m_file_int_map.contains(file)) {
    SmellIntesity si;

    si.level   = lvl;
    si.average = lvl;
    si.count   = 1;
    int indx   = static_cast<int>(lvl);
    si.level_c[(indx == 10) ? 9 : indx]++;

    m_file_int_map.insert({ file, si });
  } else {
    auto& s = m_file_int_map.at(file);
    s.level += lvl;
    s.count++;
    s.average = s.level / s.count;
    int indx  = static_cast<int>(lvl);
    s.level_c[(indx == 10) ? 9 : indx]++;
  }
}

void
SmellStatistics::ClearStats(void) {
  m_file_int_map.clear();
  m_smell_int_map.clear();
  m_overall_int = 0.0f;
}

void
SmellStatistics::DisplayGui(void) {
  ImGui::Begin("Statistics");

  ImGui::Text("%s%.2f", "Overall Intensity: ", m_overall_int);

  if (ImGui::TabItemButton("Intensity per Smell")) {
    ImGui::Separator();
    for (auto& ref : m_smell_int_map) {
      ImGui::Text("%s", std::string(ref.first).c_str());
      ImGui::Text("%s%.2f", "Smell Intensity: ", ref.second.level);
      ImGui::Text("%s%.2f", "Average Intensity: ", ref.second.average);
      ImGui::Text("%s%ld", "Smell Count: ", ref.second.count);
      if (ImGui::CollapsingHeader("Per Intensity Level Count")) {
        for (int i = 0; i < 10; i++) {
          ImGui::Text("%d%c%d%s%ld",
                      i,
                      '-',
                      i + 1,
                      ": ",
                      ref.second.level_c[i]);
        }
      }
      ImGui::Separator();
    }
  }
  if (ImGui::TabItemButton("Intensity per File")) {
    for (auto& ref : m_file_int_map) {
      ImGui::Text("%s", std::string(ref.first).c_str());
      ImGui::Text("%s%.2f", "Smell Intensity: ", ref.second.level);
      ImGui::Text("%s%.2f", "Average Intensity: ", ref.second.average);
      ImGui::Text("%s%ld", "Smell Count: ", ref.second.count);
      if (ImGui::CollapsingHeader("Per Intensity Level Count")) {
        for (int i = 0; i < 10; i++) {
          ImGui::Text("%d%c%d%s%ld",
                      i,
                      '-',
                      i + 1,
                      ": ",
                      ref.second.level_c[i]);
        }
      }
      ImGui::Separator();
    }
  }

  ImGui::End();
}

}  // namespace ICSVapp
