#include "range_based_smell_eval.hpp"
#include <assert.h>
#include <math.h>

/*
  Returns 0 if curr_lvl < minimum range
  Returns 10 if curr_lvl >= maximum range
  Else if within range calculate and return smell level
*/
auto
RangeBasedEvaluator::EvaluateSmell(unsigned curr_lvl)
    -> icsv::detector::SmellEvaluator::SmellLevel {
  return (curr_lvl < m_range.min) * 0 + (curr_lvl > m_range.max) * 10
      + IsWithinRange(curr_lvl)
      * std::round(m_range.range() / (curr_lvl - m_range.min) * 1000) / 100;
}

void
RangeBasedEvaluator::DeserializeConfig(const Json::Value& doc) {
  m_range.min = doc[m_tag]["range"]["min"].asInt();
  m_range.max = doc[m_tag]["range"]["max"].asInt();
  assert(m_range.max >= m_range.min);
}