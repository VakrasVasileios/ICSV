#include "icsv/detector/smell_evaluator.hpp"
#include "json/json.h"
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
  Json::Value det_cnfg;

  std::ifstream file(path);
  assert(file.is_open());
  file >> det_cnfg;
  assert(det_cnfg.isObject());
}

auto
SmellEvaluator::EvaluateSmell(const std::string& tag, int curr_lvl) -> int {}

}  // namespace icsv::detector