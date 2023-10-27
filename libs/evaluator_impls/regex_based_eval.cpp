#include "regex_based_eval.hpp"
#include "icsv/detector/detector_manager.hpp"
#include <assert.h>
#include <iostream>
#ifndef UNIT_TESTS
#include <imgui/imgui.h>
#endif

auto
RegexBasedEval::EvaluateClassName(const std::string& _name)
    -> icsv::detector::ISmellEvaluator::SmellLevel {
  return EvaluateName(_name, std::regex(m_class_names));
}

auto
RegexBasedEval::EvaluateMethodName(const std::string& _name)
    -> icsv::detector::ISmellEvaluator::SmellLevel {
  return EvaluateName(_name, std::regex(m_method_names));
}

auto
RegexBasedEval::EvaluateVarName(const std::string& _name)
    -> icsv::detector::ISmellEvaluator::SmellLevel {
  return EvaluateName(_name, std::regex(m_var_names));
}

auto
RegexBasedEval::ReEvaluateSmell(int)
    -> icsv::detector::ISmellEvaluator::SmellLevel {
  icsv::detector::DetectorManager::Get().UseDetectorWithTag(m_tag);
  return -1;
}

void
RegexBasedEval::DeserializeConfig(const Json::Value& doc) {
  assert(doc.isObject());

  auto convs    = doc["Naming conventions"];
  m_description = convs["description"].asString();
  for (auto iter = convs["conventions"].begin();
       iter != convs["conventions"].end();
       iter++) {
    m_conventions[iter.name()] = (*iter).asString();
  }

  m_class_names  = m_conventions[convs["class_names"].asString()];
  m_method_names = m_conventions[convs["method_names"].asString()];
  m_var_names    = m_conventions[convs["var_names"].asString()];

  m_range.min = convs["max_chars_ignored"]["min"].asInt();
  m_range.max = convs["max_chars_ignored"]["max"].asInt();
}

#define BUFF 100

void
RegexBasedEval::DisplayGui(void) {
#ifndef UNIT_TESTS
  static char cname[BUFF];
  static char mname[BUFF];
  static char vname[BUFF];
  static bool init = false;

  if (!init) {
    std::strcpy(cname, m_class_names.c_str());
    std::strcpy(mname, m_method_names.c_str());
    std::strcpy(vname, m_var_names.c_str());
    init = true;
  }

  ImGui::Text("%s", m_tag.c_str());
  ImGui::Text("%s%s", "Description: ", m_description.c_str());
  ImGui::InputInt(std::string("Min " + m_tag).c_str(), &(m_range.min));
  ImGui::InputInt(std::string("Max " + m_tag).c_str(), &(m_range.max));
  ImGui::InputText("Class name regex", cname, BUFF);
  ImGui::InputText("Method name regex", mname, BUFF);
  ImGui::InputText("Var name regex", vname, BUFF);

  if (ImGui::Button(std::string("Re-Eval " + m_tag).c_str())) {
    ReEvaluateSmell(0);
  }

  ImGui::Separator();
#endif
}

auto
RegexBasedEval::EvaluateName(const std::string& _name, std::regex exp)
    -> icsv::detector::ISmellEvaluator::SmellLevel {
  int         non_matching_chars = _name.size();
  std::smatch base_match;
  if (std::regex_match(_name, base_match, exp)) {
    std::size_t longest_match = 0;
    for (auto match : base_match) {
      if (match.str().size() > longest_match)
        longest_match = match.str().size();
    }
    non_matching_chars = static_cast<int>(_name.size()) - longest_match;
  }
  return non_matching_chars;
}