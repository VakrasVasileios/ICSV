#pragma once

#include <json/json.h>
#include <string>

namespace icsv::detector {

class SmellEvaluatorVisitor;
class RangeEvalVisitor;
class RegexEvalVisitor;

class SmellEvaluator {
public:
  using SmellLevel = int;

public:
  SmellEvaluator(const std::string& tag);
  virtual ~SmellEvaluator() = default;

  virtual void DeserializeConfig(const Json::Value& doc) = 0;

  virtual auto EvaluateSmell(int curr_lvl) -> SmellLevel   = 0;
  virtual auto ReEvaluateSmell(int init_lvl) -> SmellLevel = 0;

  virtual void DisplayGui(void) = 0;
};

}  // namespace icsv::detector