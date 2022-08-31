#include "icsv/detector/architecture_holder.hpp"
#include <assert.h>
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
  assert(file.is_open());
  file >> m_json_arch;
  assert(m_json_arch.isObject());
}

}  // namespace icsv::detector