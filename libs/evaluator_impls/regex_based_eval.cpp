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

  m_field_map[field_tag] = regex_tag;

  assert(m_field_map.at(field_tag) == regex_tag);
}

auto
RegexEvaluator::EvaluateField(const std::string& _field,
                              const std::string& _name) -> NonMatchingChars {
  return EvaluateName(_name,
                      std::regex(m_regex_map.at(m_field_map.at(_field))));
}

auto
RegexEvaluator::ReEvaluateSmell(int)
    -> icsv::detector::SmellEvaluator::SmellLevel {
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

#ifndef UNIT_TESTS
void
RegexEvaluator::FieldRegexList(const std::string& preview,
                               const std::string& field) {

  if (ImGui::BeginCombo(field.c_str(), preview.c_str())) {
    for (auto& r : m_regex_map) {
      bool is_selected = false;
      ImGui::Selectable(r.first.c_str(), &is_selected);
      if (is_selected) {
        ImGui::SetItemDefaultFocus();
        m_field_map[field] = r.first;
      }
    }
    ImGui::EndCombo();
  }
}
#endif

#define BUFF_SIZE 128

void
RegexEvaluator::DisplayGui(void) {
#ifndef UNIT_TESTS
  static char name[BUFF_SIZE], regex[BUFF_SIZE];
  static bool form_empty = false;

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

    ImGui::Separator();
    for (auto& f : m_field_map) {
      FieldRegexList(f.second, f.first);
    }
    ImGui::Separator();

    if (ImGui::Button("Add Regex"))
      ImGui::OpenPopup("New Regex");
    if (ImGui::BeginPopup("New Regex")) {

      ImGui::InputText("Name", name, BUFF_SIZE);
      ImGui::InputText("Regex", regex, BUFF_SIZE);

      if (ImGui::Button("Confirm")) {
        std::string n = name, rg = regex;
        if (n != "" && rg != "") {
          m_regex_map.insert({ n, rg });
          form_empty = false;
        } else
          form_empty = true;
      }
      if (form_empty) {
        ImGui::Text("%s", "Both fields have to be filled!");
      }

      ImGui::EndPopup();
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