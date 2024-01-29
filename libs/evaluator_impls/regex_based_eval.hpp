#pragma once

#include "range_based_eval.hpp"
#include <regex>
#include <string>
#include <map>

class RegexEvaluator final : public RangeEvaluator {
public:
  using NonMatchingChars = int;

public:
  RegexEvaluator(const std::string& _tag, std::size_t _buff_size = 100);
  ~RegexEvaluator() override = default;

  void AddField(const std::string& _tag);
  void AddRegex(const std::string& _tag, const std::string& _regex);
  void AssignRegexToField(const std::string& field_tag,
                          const std::string& regex_tag);

  auto EvaluateField(const std::string& _field, const std::string& _str)
      -> NonMatchingChars;

  auto ReEvaluateSmell(int lvl)
      -> icsv::detector::ISmellEvaluator::SmellLevel override;

  void DisplayGui(void) override;

  auto Seriallize(void) -> std::string;

  auto GetRegex(const std::string& _tag) const -> const std::string&;
  auto GetRegex(const std::string& _tag) -> std::string&;

private:
  using RegexArray = std::map<std::string, std::string>;
  using Fields     = std::map<std::string, char*>;

private:
  RegexArray  m_regex_map;
  Fields      m_field_map;
  std::size_t m_buff_size;

  auto EvaluateName(const std::string& _name, std::regex exp)
      -> NonMatchingChars;
};
