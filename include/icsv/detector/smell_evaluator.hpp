#pragma once

#include <json/json.h>
#include <string>

namespace icsv::detector {

class SmellEvaluator {
public:
  using SmellLevel = double;

public:
  SmellEvaluator(const std::string& tag);
  virtual ~SmellEvaluator() = default;

  virtual void DeserializeConfig(const Json::Value& doc) = 0;

  virtual auto EvaluateSmell(int curr_lvl) -> SmellLevel = 0;
};

}  // namespace icsv::detector