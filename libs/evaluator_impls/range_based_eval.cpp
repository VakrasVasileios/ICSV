#include "range_based_eval.hpp"
#include "icsv/detector/report_center.hpp"
#include <assert.h>
#ifndef UNIT_TESTS
#include <imgui/imgui.h>
#endif

RangeEvaluator::RangeEvaluator(const std::string& _tag, int _min, int _max)
    : icsv::detector::ISmellEvaluator(_tag), m_range(_min, _max) {
  m_type = icsv::detector::EvalType::RANGE;
}

/*
  Returns 0 if curr_lvl < minimum range
  Returns 10 if curr_lvl >= maximum range
  Else if within range calculate and return smell level
*/
auto
RangeEvaluator::EvaluateSmell(int curr_lvl)
    -> icsv::detector::ISmellEvaluator::SmellLevel {
  return (
      (curr_lvl < m_range.min) * 0.00f + (curr_lvl >= m_range.max) * 10.00f
      + IsWithinRange(curr_lvl)
          * (((double) (curr_lvl - m_range.min) / m_range.range()) * 10.00f));
}

auto
RangeEvaluator::ReEvaluateSmell(int init_lvl)
    -> icsv::detector::ISmellEvaluator::SmellLevel {
  return EvaluateSmell(init_lvl);
}

void
RangeEvaluator::SetRange(int min, int max) {
  assert(min < max);
  m_range.min = min;
  m_range.max = max;
}

void
RangeEvaluator::SetLimits(int min, int max) {
  assert(min < max);
  m_limits.min = min;
  m_limits.max = max;
}

auto
RangeEvaluator::Seriallize(void) -> std::string {
  auto ser = ISmellEvaluator::Seriallize();
  ser += ",\n\"range\":{\"min\":" + std::to_string(m_range.min)
      + ",\"max\":" + std::to_string(m_range.max) + "}";
  ser += ",\n\"limits\":{\"min\":" + std::to_string(m_limits.min)
      + ",\"max\":" + std::to_string(m_limits.max) + "}";

  return ser;
}

void
RangeEvaluator::DisplayGui(void) {
#ifndef UNIT_TESTS
  ImGui::Separator();
  if (ImGui::TreeNode(m_tag.c_str())) {
    ImGui::Separator();
    ImGui::Text("%s%s", "Description: ", m_description.c_str());
    ImGui::Separator();
    ImGui::Text("%s", "Range: ");
    ImGui::SliderInt(std::string("Min " + m_tag).c_str(),
                     &m_range.min,
                     m_limits.min,
                     m_range.max);
    ImGui::SliderInt(std::string("Max " + m_tag).c_str(),
                     &m_range.max,
                     m_range.min,
                     m_limits.max);
    ImGui::Separator();

    if (ImGui::Button(std::string("Re-Eval " + m_tag).c_str())) {
      for (auto* rep :
           icsv::detector::ReportCenter::Get().GetReportsByTag(m_tag))
        rep->level = ReEvaluateSmell(rep->init_level);
    }
    ImGui::TreePop();
    ImGui::Separator();
  }
#endif
}