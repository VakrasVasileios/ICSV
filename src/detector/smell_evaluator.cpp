#include "icsv/detector/smell_evaluator.hpp"
#include "icsv/detector/evaluation_center.hpp"
#include <assert.h>

namespace icsv::detector {

ISmellEvaluator::ISmellEvaluator(const std::string& tag) {
  assert(tag != "");
  m_tag = tag;
  register_evaluator(tag, this);
  assert(m_tag != "");
}

void
ISmellEvaluator::SetDescription(const std::string& dsc) {
  m_description = dsc;
}

void
ISmellEvaluator::SetTag(const std::string& t) {
  m_tag = t;
}

auto
ISmellEvaluator::GetDescription(void) const -> const std::string& {
  return m_description;
}

auto
ISmellEvaluator::GetTag(void) const -> const std::string& {
  return m_tag;
}

}  // namespace icsv::detector