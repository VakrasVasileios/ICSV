#include "icsv/detector/smell_evaluator.hpp"
#include <assert.h>
#include <fstream>

namespace icsv::detector {

auto
SmellEvaluator::Get() -> SmellEvaluator& {
  static SmellEvaluator singleton;
  return singleton;
}

void
SmellEvaluator::DeserializeSmellConfig(const std::string& path) {
  std::ifstream file(path);
  assert(file.is_open());
  file >> m_smell_cnfg;
  assert(m_smell_cnfg.isObject());
}

auto
SmellEvaluator::EvaluateSmell(const std::string& tag, int curr_lvl)
    -> int {  // TODO: Get a sample and use it unit tests and this
              // implementation
}

}  // namespace icsv::detector