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

  void SetDisplayGuiFunc(const GuiFunc& f) { m_displayGui = f; }

  void DisplayGui(void);

private:
  GuiFunc m_displayGui;
};

}  // namespace icsv::detector