#include "range_based_eval.hpp"
#include "icsv/detector/report_center.hpp"
#include <assert.h>
#ifndef UNIT_TESTS
#include <imgui/imgui.h>
#endif

/*
  Returns 0 if curr_lvl < minimum range
  Returns 10 if curr_lvl >= maximum range
  Else if within range calculate and return smell level
*/
auto
RangeBasedEvaluator::EvaluateSmell(int curr_lvl)
    -> icsv::detector::ISmellEvaluator::SmellLevel {
  return (curr_lvl < m_range.min) * 0 + (curr_lvl >= m_range.max) * 10
      + IsWithinRange(curr_lvl)
      * (((double) (curr_lvl - m_range.min) / m_range.range()) * 10);
}

auto
RangeBasedEvaluator::ReEvaluateSmell(int init_lvl)
    -> icsv::detector::ISmellEvaluator::SmellLevel {
  return EvaluateSmell(init_lvl);
}

void
RangeBasedEvaluator::DeserializeConfig(const Json::Value& doc) {
  m_description = doc[m_tag]["description"].asString();
  m_range.min   = doc[m_tag]["range"]["min"].asInt();
  m_range.max   = doc[m_tag]["range"]["max"].asInt();
  assert(m_range.max >= m_range.min);
}

void
RangeBasedEvaluator::DisplayGui(void) {
#ifndef UNIT_TESTS
  ImGui::Separator();
  if (ImGui::TreeNode(m_tag.c_str())) {
    ImGui::Separator();
    ImGui::Text("%s%s", "Description: ", m_description.c_str());
    ImGui::Separator();
    ImGui::Text("%s", "Range: ");
    ImGui::InputInt(std::string("Min " + m_tag).c_str(), &(m_range.min));
    ImGui::InputInt(std::string("Max " + m_tag).c_str(), &(m_range.max));
    ImGui::Separator();

    if (ImGui::Button(std::string("Re-Eval " + m_tag).c_str())) {
      for (auto* rep :
           icsv::detector::ReportCenter::Get().GetReportsByTag(m_tag))
        ReEvaluateSmell(rep->init_level);
    }
    ImGui::TreePop();
    ImGui::Separator();
  }
#endif
}