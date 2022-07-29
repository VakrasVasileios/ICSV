#include "icsv/detector/architecture_holder.hpp"
#include <fstream>

namespace icsv::detector {

auto
ArchitectureHolder::Get() -> ArchitectureHolder& {
  static ArchitectureHolder singleton;
  return singleton;
}

void
ArchitectureHolder::DeserializeArchitecture(const std::string& path) {
  std::ifstream file(path);
  file >> m_json_arch;
}

}  // namespace icsv::detector