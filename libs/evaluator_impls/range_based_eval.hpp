#pragma once

#include "icsv/detector/smell_evaluator.hpp"

class RangeBasedEvaluator : public icsv::detector::SmellEvaluator {
public:
  RangeBasedEvaluator(const std::string& tag)
      : icsv::detector::SmellEvaluator(tag), m_tag(tag) {}
  ~RangeBasedEvaluator() override = default;

  auto EvaluateSmell(int curr_lvl)
      -> icsv::detector::SmellEvaluator::SmellLevel override;

  virtual void DeserializeConfig(const Json::Value& doc) override;

protected:
  struct Range {
    int min, max;
    int range() { return max - min; }
  } m_range;

private:
  std::string m_tag;

  bool IsWithinRange(int curr_lvl) {
    return curr_lvl >= m_range.min && curr_lvl <= m_range.max;
  }
};

inline auto
CreateRangeBasedEval(const std::string& name) -> RangeBasedEvaluator* {
  return new RangeBasedEvaluator{ name };
}

#define CREATE_RANGE_BASED_EVAL(tag)                   \
  namespace {                                          \
  static const auto* eval = CreateRangeBasedEval(tag); \
  }
