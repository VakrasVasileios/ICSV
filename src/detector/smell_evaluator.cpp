#include "icsv/detector/smell_evaluator.hpp"
#include "icsv/detector/evaluation_center.hpp"
#include <assert.h>

namespace icsv::detector {

SmellEvaluator::SmellEvaluator(const std::string& tag) {
  assert(tag != "");
  m_tag = tag;
  register_evaluator(tag, this);
  assert(m_tag != "");
}

auto
SmellEvaluator::Seriallize(void) -> std::string {
  std::string ser{ "" };
  ser += "\"tag\":\"" + m_tag + "\"";
  ser += ",\n\"description\":\"" + m_description + "\"";
  switch (m_type) {
    case RANGE:
      ser += ",\n\"type\":\"range\"";
      break;
    case REGEX:
      ser += ",\n\"type\":\"regex\"";
      break;
    case BOOL:
      ser += ",\n\"type\":\"bool\"";
      break;
    case ARGS:
      ser += ",\n\"type\":\"args\"";
      break;
    case NONE:
    default:
      assert(false);
      break;
  }

  return ser;
}

void
SmellEvaluator::SetDescription(const std::string& dsc) {
  m_description = dsc;
}

void
SmellEvaluator::SetTag(const std::string& t) {
  m_tag = t;
}

auto
SmellEvaluator::GetDescription(void) const -> const std::string& {
  return m_description;
}

auto
SmellEvaluator::GetTag(void) const -> const std::string& {
  return m_tag;
}

}  // namespace icsv::detector