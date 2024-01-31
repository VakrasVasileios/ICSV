#pragma once

#include <json/json.h>
#include <string>

namespace icsv::detector {

enum EvalType { NONE, RANGE, REGEX, BOOL, ARGS };

class SmellEvaluator {
public:
  using SmellLevel = float;

public:
  SmellEvaluator(const std::string& tag);
  virtual ~SmellEvaluator() = default;

  virtual auto EvaluateSmell(int curr_lvl) -> SmellLevel   = 0;
  virtual auto ReEvaluateSmell(int init_lvl) -> SmellLevel = 0;

  void SetDescription(const std::string& dsc);
  void SetTag(const std::string& t);

  auto GetDescription(void) const -> const std::string&;
  auto GetDescription(void) -> std::string { return m_description; }
  auto GetTag(void) const -> const std::string&;
  auto GetTag(void) -> std::string { return m_tag; }

  auto GetType(void) -> EvalType { return m_type; }

  virtual void DisplayGui(void) = 0;

  virtual auto Seriallize(void) -> std::string;

protected:
  std::string m_tag         = "";
  std::string m_description = "";
  EvalType    m_type        = NONE;
};

}  // namespace icsv::detector