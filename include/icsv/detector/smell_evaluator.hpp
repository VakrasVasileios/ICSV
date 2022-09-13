#pragma once

#include <json/json.h>
#include <string>

namespace icsv::detector {

class SmellEvaluator {
public:
  using SmellLevel = unsigned;

public:
  SmellEvaluator(const std::string& tag);
  virtual ~SmellEvaluator() = default;

  virtual void DeserializeSmellConfig(const Json::Value& doc) = 0;

  virtual auto EvaluateSmell(unsigned curr_lvl) -> SmellLevel = 0;
};

}  // namespace icsv::detector