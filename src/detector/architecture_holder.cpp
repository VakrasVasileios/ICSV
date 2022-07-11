#include "icsv/detector/architecture_holder.hpp"

namespace detector {

auto
ArchitectureHolder::Get() -> ArchitectureHolder& {
  static ArchitectureHolder singleton;
  return singleton;
}

void
ArchitectureHolder::ReadArchitectureFile(const std::string& path) {}

}  // namespace detector