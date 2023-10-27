#pragma once

#include <json/json.h>
#include <string>
#include <functional>

namespace icsv::detector {

using GuiFunc = std::function<void(void)>;

class SmellEvaluator {
public:
  using SmellLevel = int;

public:
  SmellEvaluator(const std::string& tag);
  virtual ~SmellEvaluator() = default;

  virtual void DeserializeConfig(const Json::Value& doc) = 0;

  virtual auto EvaluateSmell(int curr_lvl) -> SmellLevel   = 0;
  virtual auto ReEvaluateSmell(int init_lvl) -> SmellLevel = 0;

  void SetDisplayGuiFunc(const GuiFunc& f);
  void SetDescription(const std::string& dsc);

  auto GetDescription(void) const -> const std::string&;

  virtual void DisplayGui(void) = 0;

private:
  GuiFunc     m_displayGui;
  std::string m_description = "";
};

}  // namespace icsv::detector