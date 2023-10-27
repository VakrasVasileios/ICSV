#pragma once

#include "range_based_eval.hpp"
#include <regex>
#include <string>
#include <unordered_map>

class RegexBasedEval final : public RangeBasedEvaluator {
public:
  RegexBasedEval(const std::string& _tag) : RangeBasedEvaluator(_tag) {}
  ~RegexBasedEval() override = default;

  auto EvaluateClassName(const std::string& _name)
      -> icsv::detector::ISmellEvaluator::SmellLevel;
  auto EvaluateMethodName(const std::string& _name)
      -> icsv::detector::ISmellEvaluator::SmellLevel;
  auto EvaluateVarName(const std::string& _name)
      -> icsv::detector::ISmellEvaluator::SmellLevel;

  auto EvaluateSmell(int) -> icsv::detector::ISmellEvaluator::SmellLevel {
    return -1;
  }

  auto ReEvaluateSmell(int lvl)
      -> icsv::detector::ISmellEvaluator::SmellLevel override;

  void DeserializeConfig(const Json::Value& doc) override;

  void DisplayGui(void) override;

private:
  using NamingConventions = std::unordered_map<std::string, std::string>;

private:
  NamingConventions m_conventions;
  std::string       m_class_names;
  std::string       m_method_names;
  std::string       m_var_names;

  auto EvaluateName(const std::string& _name, std::regex exp)
      -> icsv::detector::ISmellEvaluator::SmellLevel;
};

inline auto
CreateRegexBasedEval(const std::string& tag) {
  return new RegexBasedEval(tag);
}

#define CREATE_REGEX_BASED_EVAL(tag)             \
  namespace {                                    \
  static auto* eval = CreateRegexBasedEval(tag); \
  }
