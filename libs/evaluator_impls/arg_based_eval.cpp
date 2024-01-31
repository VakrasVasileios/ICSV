#include "./arg_based_eval.hpp"
#include "./range_based_eval.hpp"
#include "./regex_based_eval.hpp"
#include "icsv/detector/detector_manager.hpp"
#include <assert.h>

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
      auto* rgx_eval = dynamic_cast<RegexEvaluator*>(eval);
      return rgx_eval->EvaluateSmell(init_lvl);
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
      icsv::detector::DetectorManager::Get().UseDetectorWithTag(m_tag);
    }
    ImGui::TreePop();
    ImGui::Separator();
  }
#endif
}