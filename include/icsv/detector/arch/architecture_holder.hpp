#pragma once

#include "architecture_data.hpp"
#include <string>

#define DESERIALLIZE_ARCH_CONF(path) \
  icsv::detector::arch::ArchitectureHolder::Get().DeserializeArchitecture(path)

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
