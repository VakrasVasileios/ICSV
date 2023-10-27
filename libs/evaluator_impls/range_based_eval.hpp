#pragma once

#include "icsv/detector/smell_evaluator.hpp"

class RangeBasedEvaluator : public icsv::detector::ISmellEvaluator {
public:
  RangeBasedEvaluator(const std::string& tag)
      : icsv::detector::ISmellEvaluator(tag) {}
  ~RangeBasedEvaluator() override = default;

  virtual auto EvaluateSmell(int curr_lvl)
      -> icsv::detector::ISmellEvaluator::SmellLevel override;

  virtual auto ReEvaluateSmell(int init_lvl)
      -> icsv::detector::ISmellEvaluator::SmellLevel override;

  virtual void DeserializeConfig(const Json::Value& doc) override;

  virtual void DisplayGui(void) override;

protected:
  struct Range {
    int min, max;
    int range() { return max - min; }
  } m_range;

private:
  // curr_lvl within [min,max)
  bool IsWithinRange(int curr_lvl) {
    return curr_lvl >= m_range.min && curr_lvl < m_range.max;
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
