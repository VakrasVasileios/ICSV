#pragma once

#include <json/json.h>
#include <string>
#include <functional>

namespace icsv::detector {

using GuiFunc = std::function<void(void)>;

class ISmellEvaluator {
public:
  using SmellLevel = int;

public:
  ISmellEvaluator(const std::string& tag);
  virtual ~ISmellEvaluator() = default;

  virtual void DeserializeConfig(const Json::Value& doc) = 0;

  virtual auto EvaluateSmell(int curr_lvl) -> SmellLevel   = 0;
  virtual auto ReEvaluateSmell(int init_lvl) -> SmellLevel = 0;

  void SetDisplayGuiFunc(const GuiFunc& f);
  void SetDescription(const std::string& dsc);
  void SetTag(const std::string& t);

  auto GetDescription(void) const -> const std::string&;
  auto GetTag(void) const -> const std::string&;

  virtual void DisplayGui(void) = 0;

protected:
  std::string m_tag         = "";
  std::string m_description = "";

private:
  GuiFunc m_displayGui;
};

}  // namespace icsv::detector