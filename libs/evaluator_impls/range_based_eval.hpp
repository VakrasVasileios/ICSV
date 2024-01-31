#pragma once

#include "icsv/detector/smell_evaluator.hpp"

class RangeEvaluator : public icsv::detector::SmellEvaluator {
public:
  RangeEvaluator(const std::string& _tag, int _min = 1, int _max = 10);
  ~RangeEvaluator() override = default;

  virtual auto EvaluateSmell(int curr_lvl)
      -> icsv::detector::SmellEvaluator::SmellLevel override;

  void SetRange(int min, int max);

  virtual auto ReEvaluateSmell(int init_lvl)
      -> icsv::detector::SmellEvaluator::SmellLevel override;

  virtual void DisplayGui(void) override;

  virtual auto Seriallize(void) -> std::string;

protected:
  struct Range {
    int min, max;

    auto range() -> int { return max - min; }

    Range(int _min = 0, int _max = 0) : min(_min), max(_max) {}
  } m_range;

  void AlwaysGEzero(int& num) { num = num * (num >= 0); }

private:
  // curr_lvl within [min,max)
  bool IsWithinRange(int curr_lvl) {
    return curr_lvl >= m_range.min && curr_lvl < m_range.max;
  }
};
