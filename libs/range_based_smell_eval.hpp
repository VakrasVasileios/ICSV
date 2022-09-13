#pragma once

#include "icsv/detector/smell_evaluator.hpp"

class RangeBasedEvaluator : public icsv::detector::SmellEvaluator {
public:
  RangeBasedEvaluator(const std::string& tag)
      : icsv::detector::SmellEvaluator(tag), m_tag(tag) {}
  ~RangeBasedEvaluator() override = default;

  auto EvaluateSmell(unsigned curr_lvl)
      -> icsv::detector::SmellEvaluator::SmellLevel override;

  void DeserializeConfig(const Json::Value& doc) override;

private:
  std::string m_tag;
  struct Range {
    int min, max;
    int range() { return max - min; }
  } m_range;

  bool IsWithinRange(unsigned curr_lvl) {
    return curr_lvl >= m_range.min && curr_lvl <= m_range.max;
  }
};

inline auto
CreateRangeBasedEval(const std::string& name) -> RangeBasedEvaluator* {
  return new RangeBasedEvaluator{ name };
}

#define CREATE_RANGE_BASED_EVAL(tag) \
  { static const auto* eval = CreateRangeBasedEval(tag); }