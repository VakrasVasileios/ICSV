#include "icsv/detector/smell_evaluator.hpp"
#include "icsv/detector/evaluation_center.hpp"

namespace icsv::detector {

SmellEvaluator::SmellEvaluator(const std::string& tag) {
  register_evaluator(tag, this);
}

void
SmellEvaluator::DisplayGui(void) {
  if (!m_displayGui)
    m_displayGui();
}

}  // namespace icsv::detector