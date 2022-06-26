#include "detector/detector_manager.hpp"

namespace detector {

DetectorManager::~DetectorManager() {}

auto
DetectorManager::Get() -> DetectorManager& {
  static DetectorManager singleton;
  return singleton;
}

}  // namespace detector