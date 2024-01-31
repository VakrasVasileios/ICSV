#include "icsv/detector/detector.hpp"

namespace icsv::detector {

auto
Detector::GetEvaluator(void) -> SmellEvaluator* {
  return EvaluationCenter::Get().GetEvaluator(m_tag);
}

}  // namespace icsv::detector