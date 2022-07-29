#pragma once

#include <fstream>
#include <json/json.h>
#include <string>

namespace icsv::detector {

class ArchitectureHolder final {
public:
  static auto Get() -> ArchitectureHolder&;

  void DeserializeArchitecture(const std::string& path);

  auto GetArchitecture(void) const -> const Json::Value& { return m_json_arch; }

private:
  ArchitectureHolder()                          = default;
  ArchitectureHolder(const ArchitectureHolder&) = delete;
  ArchitectureHolder(ArchitectureHolder&&)      = delete;
  ~ArchitectureHolder()                         = default;

  Json::Value m_json_arch;
};

}  // namespace icsv::detector
