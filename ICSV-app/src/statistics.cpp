#include "statistics.hpp"
#include <imgui/imgui.h>

namespace ICSVapp {

auto
SmellStatistics::Get(void) -> SmellStatistics& {
  static SmellStatistics singleton;
  return singleton;
}

void
SmellStatistics::AddSmellIntensity(const char* tag, float lvl) {
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
SmellStatistics::AddFileIntensity(const std::string& file,
                                  const char*        smell_tag,
                                  float              lvl) {
  if (!m_file_int_map.contains(file)) {
    FileIntensity fi;

    fi.smell_count[smell_tag] = 0;

    fi.si.level   = lvl;
    fi.si.average = lvl;
    fi.si.count   = 1;
    int indx      = static_cast<int>(lvl);
    fi.si.level_c[(indx == 10) ? 9 : indx]++;

    m_file_int_map.insert({ file, fi });
  } else {
    auto& f = m_file_int_map.at(file);

    f.smell_count[smell_tag]++;

    f.si.level += lvl;
    f.si.count++;
    f.si.average = f.si.level / f.si.count;
    int indx     = static_cast<int>(lvl);
    f.si.level_c[(indx == 10) ? 9 : indx]++;
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
  static ImGuiTableFlags flags
      = ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg;

  ImGui::Begin("Statistics");

  ImGui::Text("%s%.2f", "Overall Intensity: ", m_overall_int);
  ImGui::Separator();

  if (ImGui::BeginTabBar("##tabs", ImGuiTabBarFlags_None)) {

    if (ImGui::BeginTabItem("Intensity per Smell")) {
      for (auto& ref : m_smell_int_map) {
        ImGui::Text("%s", std::string(ref.first).c_str());
        ImGui::Text("%s%.2f", "Smell Intensity: ", ref.second.level);
        ImGui::Text("%s%.2f", "Average Intensity: ", ref.second.average);
        ImGui::Text("%s%ld", "Smell Count: ", ref.second.count);

        if (ImGui::BeginTable("#tableSmell", 10, flags)) {
          ImGui::TableSetupColumn("0 - 1");
          ImGui::TableSetupColumn("1 - 2");
          ImGui::TableSetupColumn("2 - 3");
          ImGui::TableSetupColumn("3 - 4");
          ImGui::TableSetupColumn("4 - 5");
          ImGui::TableSetupColumn("5 - 6");
          ImGui::TableSetupColumn("6 - 7");
          ImGui::TableSetupColumn("7 - 8");
          ImGui::TableSetupColumn("8 - 9");
          ImGui::TableSetupColumn("9 - 10");
          ImGui::TableHeadersRow();

          ImGui::TableNextRow();
          for (int i = 0; i < 10; i++) {
            ImGui::TableSetColumnIndex(i);
            ImGui::Text("%ld", ref.second.level_c[i]);
          }

          ImGui::EndTable();
        }

        ImGui::Separator();
      }

      ImGui::EndTabItem();
    }
    if (ImGui::BeginTabItem("Intensity per File")) {
      for (auto& ref : m_file_int_map) {
        ImGui::Text("%s", std::string(ref.first).c_str());
        ImGui::Text("%s%.2f", "Smell Intensity: ", ref.second.si.level);
        ImGui::Text("%s%.2f", "Average Intensity: ", ref.second.si.average);
        ImGui::Text("%s%ld", "Smell Count: ", ref.second.si.count);

        if (ImGui::BeginTable("#tableSmellCount", 2, flags)) {
          ImGui::TableSetupColumn("Smell");
          ImGui::TableSetupColumn("Count");
          ImGui::TableHeadersRow();

          ImGui::TableNextRow();
          for (auto& s : ref.second.smell_count) {
            ImGui::TableSetColumnIndex(0);
            ImGui::Text("%s", s.first);
            ImGui::TableSetColumnIndex(1);
            ImGui::Text("%ld", s.second);
            ImGui::TableNextRow();
          }
          ImGui::EndTable();
        }

        if (ImGui::BeginTable("#tableFile", 10, flags)) {
          ImGui::TableSetupColumn("0 - 1");
          ImGui::TableSetupColumn("1 - 2");
          ImGui::TableSetupColumn("2 - 3");
          ImGui::TableSetupColumn("3 - 4");
          ImGui::TableSetupColumn("4 - 5");
          ImGui::TableSetupColumn("5 - 6");
          ImGui::TableSetupColumn("6 - 7");
          ImGui::TableSetupColumn("7 - 8");
          ImGui::TableSetupColumn("8 - 9");
          ImGui::TableSetupColumn("9 - 10");
          ImGui::TableHeadersRow();

          ImGui::TableNextRow();
          for (int i = 0; i < 10; i++) {
            ImGui::TableSetColumnIndex(i);
            ImGui::Text("%ld", ref.second.si.level_c[i]);
          }

          ImGui::EndTable();
        }

        ImGui::Separator();
      }

      ImGui::EndTabItem();
    }

    ImGui::EndTabBar();
  }

  ImGui::End();
}

}  // namespace ICSVapp
