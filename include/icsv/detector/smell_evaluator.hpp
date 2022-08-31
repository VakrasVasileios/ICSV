#pragma once

#include "json/json.h"
#include <string>

#define EVAL_SMELL(tag, curr_lvl) \
  icsv::detector::SmellEvaluator::Get().EvaluateSmell(tag, curr_lvl);

namespace icsv::detector {

class SmellEvaluator final {
public:
  static auto Get() -> SmellEvaluator&;

  void DeserializeSmellConfig(const std::string& path);

  auto EvaluateSmell(const std::string& tag, int curr_lvl) -> int;

private:
  Json::Value m_smell_cnfg;

  SmellEvaluator()                      = default;
  SmellEvaluator(const SmellEvaluator&) = delete;
  SmellEvaluator(SmellEvaluator&&)      = delete;
  ~SmellEvaluator()                     = default;
};

}  // namespace icsv::detector