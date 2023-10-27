#include "icsv/detector/smell_evaluator.hpp"
#include "icsv/detector/evaluation_center.hpp"

namespace icsv::detector {

SmellEvaluator::SmellEvaluator(const std::string& tag) {
  register_evaluator(tag, this);
}

void
SmellEvaluator::SetDisplayGuiFunc(const GuiFunc& f) {
  m_displayGui = f;
}
void
SmellEvaluator::SetDescription(const std::string& dsc) {
  m_description = dsc;
}

auto
SmellEvaluator::GetDescription(void) const -> const std::string& {
  return m_description;
}

void
SmellEvaluator::DisplayGui(void) {
  if (!m_displayGui)
    m_displayGui();
}

}  // namespace icsv::detector