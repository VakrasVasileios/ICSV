#pragma once

#include "architecture_data.hpp"
#include <string>

namespace icsv::detector::arch {

class ArchitectureHolder final {
public:
  static auto Get() -> ArchitectureHolder&;

  void DeserializeArchitecture(const std::string& path);

  auto GetArchitecture(void) const -> const ArchData& { return m_data; }

private:
  ArchitectureHolder()                          = default;
  ArchitectureHolder(const ArchitectureHolder&) = delete;
  ArchitectureHolder(ArchitectureHolder&&)      = delete;
  ~ArchitectureHolder()                         = default;

  ArchData m_data;
};

}  // namespace icsv::detector::arch
