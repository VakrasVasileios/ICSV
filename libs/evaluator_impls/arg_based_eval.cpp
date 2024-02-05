#include "./arg_based_eval.hpp"
#include "./range_based_eval.hpp"
#include "./regex_based_eval.hpp"
#include "icsv/detector/detector_manager.hpp"
#include "icsv/detector/report_center.hpp"
#include <assert.h>
#include <iostream>

#ifndef UNIT_TESTS
#include <imgui/imgui.h>
#endif

MultiArgsEvaluator::MultiArgsEvaluator(const std::string& _tag)
    : icsv::detector::SmellEvaluator(_tag) {
  m_type = icsv::detector::EvalType::ARGS;
}

MultiArgsEvaluator::~MultiArgsEvaluator() {
  for (auto ref : m_args) {
    delete ref.second;
    ref.second = nullptr;
  }
  m_args.clear();
}

void
MultiArgsEvaluator::AddEvaluator(icsv::detector::SmellEvaluator* eval) {
  assert(!m_args.contains(eval->GetTag()));

  m_args.insert({ eval->GetTag(), eval });
}

auto
MultiArgsEvaluator::EvaluateSmell(const std::string& eval_name, int init_lvl)
    -> SmellLevel {
  assert(m_args.contains(eval_name));
  auto* eval = m_args.at(eval_name);

  switch (eval->GetType()) {
    case icsv::detector::RANGE:
    case icsv::detector::BOOL: {
      auto* rng_eval = dynamic_cast<RangeEvaluator*>(eval);
      return rng_eval->EvaluateSmell(init_lvl);
    }
    case icsv::detector::REGEX: {
      std::cout << "\tWarning: Range evaluation method being used for regex "
                << "evaluation!\n";
    }
    case icsv::detector::NONE:
    case icsv::detector::ARGS:
    default:
      return -1;
  }
}

auto
MultiArgsEvaluator::EvaluateSmell(const std::string& eval_name,
                                  const std::string& field,
                                  const std::string& str) -> SmellLevel {
  assert(m_args.contains(eval_name));
  auto* eval = m_args.at(eval_name);

  auto* dc_eval = dynamic_cast<RegexEvaluator*>(eval);
  assert(dc_eval);

  return dc_eval->EvaluateField(field, str);
}

auto
MultiArgsEvaluator::ReEvaluateSmell(int) -> SmellLevel {
  icsv::detector::ReportCenter::Get().ClearReportsWithTag(m_tag);
  icsv::detector::DetectorManager::Get().UseDetectorWithTag(m_tag);
  return -1;
}

auto
MultiArgsEvaluator::Seriallize(void) -> std::string {
  auto ser = SmellEvaluator::Seriallize();
  ser += ",\"args\":[";
  for (auto ev : m_args) {
    ser += "\n{";
    ser += ev.second->Seriallize();
    ser += "},";
  }

  ser.pop_back();
  ser += "]";

  return ser;
}

void
MultiArgsEvaluator::DisplayGui(void) {
#ifndef UNIT_TESTS
  ImGui::Separator();
  if (ImGui::TreeNode(m_tag.c_str())) {
    ImGui::Separator();
    ImGui::Text("%s%s", "Description: ", m_description.c_str());
    ImGui::Separator();

    for (auto ref : m_args) {
      ref.second->DisplayGui();
    }

    if (ImGui::Button(std::string("Re-Eval " + m_tag).c_str())) {
      ReEvaluateSmell(0);
    }
    ImGui::TreePop();
    ImGui::Separator();
  }
#endif
}