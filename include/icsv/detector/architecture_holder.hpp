#pragma once

#include <json/json.h>

namespace detector {

class ArchitectureHolder final {
public:
  static auto Get() -> ArchitectureHolder&;

  void ReadArchitectureFile(const std::string& path);

private:
  ArchitectureHolder()                          = default;
  ArchitectureHolder(const ArchitectureHolder&) = delete;
  ArchitectureHolder(ArchitectureHolder&&)      = delete;
  ~ArchitectureHolder()                         = default;
};

}  // namespace detector
