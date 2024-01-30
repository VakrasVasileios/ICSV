#include "regex_based_eval.hpp"
#include "icsv/detector/detector_manager.hpp"
#include <assert.h>
#include <iostream>
#include <list>
#ifndef UNIT_TESTS
#include <imgui/imgui.h>
#endif

RegexEvaluator::RegexEvaluator(const std::string& _tag, std::size_t _buff_size)
    : RangeEvaluator(_tag), m_buff_size(_buff_size) {
  assert(m_buff_size == _buff_size);
  assert(m_buff_size != 0);
  m_type = icsv::detector::EvalType::REGEX;
}

void
RegexEvaluator::AddField(const std::string& _tag) {
  m_field_map[_tag] = new char[m_buff_size];
  assert(m_field_map.contains(_tag));
}

void
RegexEvaluator::AddRegex(const std::string& _tag, const std::string& _regex) {
  m_regex_map[_tag] = _regex;
  assert(m_regex_map.contains(_tag));
  assert(m_regex_map.at(_tag) == _regex);
}

void
RegexEvaluator::AssignRegexToField(const std::string& field_tag,
                                   const std::string& regex_tag) {
  assert(m_field_map.contains(field_tag));
  assert(m_regex_map.contains(regex_tag));

  auto field = m_field_map.at(field_tag);
  auto regex = m_regex_map.at(regex_tag);
  std::strcpy(field, regex.c_str());

  assert(m_field_map.at(field_tag) == m_regex_map.at(regex_tag));
}

auto
RegexEvaluator::EvaluateField(const std::string& _field,
                              const std::string& _name) -> NonMatchingChars {
  return EvaluateName(_name, std::regex(m_field_map[_field]));
}

auto
RegexEvaluator::ReEvaluateSmell(int)
    -> icsv::detector::ISmellEvaluator::SmellLevel {
  icsv::detector::DetectorManager::Get().UseDetectorWithTag(m_tag);
  return -1;
}

auto
RegexEvaluator::Seriallize(void) -> std::string {
  auto ser = RangeEvaluator::Seriallize();
  ser += ",\n\"regex_array\":[\n";
  bool init = false;
  for (auto& reg : m_regex_map) {
    if (init)
      ser += ",\n";
    ser += "{\"tag\":\"" + reg.first + "\"";
    ser += ",\n\"regex\":\"" + reg.second + "\"}";
    init = true;
  }
  init = false;

  ser += ']';
  ser += ",\"fields\":[\n";
  for (auto& f : m_field_map) {
    if (init)
      ser += ",\n";
    ser += "{\"tag\":\"" + f.first + "\"";
    ser += ",\n\"assigned_regex\":\"" + std::string(f.second) + "\"}";
    init = true;
  }
  ser += ']';

  return ser;
}

void
RegexEvaluator::DisplayGui(void) {
#ifndef UNIT_TESTS
  ImGui::Separator();
  if (ImGui::TreeNode(m_tag.c_str())) {
    ImGui::Separator();
    ImGui::Text("%s%s", "Description: ", m_description.c_str());
    ImGui::Separator();
    ImGui::Text("%s", "Chars Ignored: ");
    ImGui::InputInt(std::string("Min " + m_tag).c_str(), &m_range.min);
    AlwaysGEzero(m_range.min);
    ImGui::InputInt(std::string("Max " + m_tag).c_str(), &m_range.max);
    AlwaysGEzero(m_range.max);
    for (auto f : m_field_map) {
      ImGui::Separator();
      ImGui::InputText(f.first.c_str(), f.second, m_buff_size);
      ImGui::Separator();
    }

    if (ImGui::Button(std::string("Re-Eval " + m_tag).c_str())) {
      ReEvaluateSmell(0);
    }
    ImGui::TreePop();
    ImGui::Separator();
  }
#endif
}

auto
RegexEvaluator::GetRegex(const std::string& _tag) const -> const std::string& {
  assert(m_regex_map.contains(_tag));
  return m_regex_map.at(_tag);
}

auto
RegexEvaluator::GetRegex(const std::string& _tag) -> std::string& {
  assert(m_regex_map.contains(_tag));
  return m_regex_map.at(_tag);
}

auto
RegexEvaluator::EvaluateName(const std::string& _name, std::regex exp)
    -> RegexEvaluator::NonMatchingChars {
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