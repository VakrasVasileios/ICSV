#include "detector/detector_manager.hpp"
#include "detector/detector.hpp"

namespace detector {

DetectorManager::~DetectorManager() {
  for (auto* det : m_detectors)
    delete det;
}

auto
DetectorManager::Get() -> DetectorManager& {
  static DetectorManager singleton;
  return singleton;
}

void
DetectorManager::RegisterDetector(Detector* det) {
  m_detectors.push_back(det);
}

}  // namespace detector