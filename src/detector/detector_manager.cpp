#include "icsv/detector/detector_manager.hpp"
#include "icsv/detector/detector.hpp"

namespace icsv::detector {

DetectorManager::~DetectorManager() { m_detectors.clear(); }

auto
DetectorManager::Get() -> DetectorManager& {
  static DetectorManager singleton;
  return singleton;
}

void
DetectorManager::RegisterDetector(const std::string& tag, const Detector& det) {
  m_detectors[tag] = det;
}

}  // namespace icsv::detector