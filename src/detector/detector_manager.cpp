#include "icsv/detector/detector_manager.hpp"
#include "icsv/detector/detector.hpp"

namespace icsv::detector {

DetectorManager::~DetectorManager() {
  for (auto d : m_detectors)
    delete d.second;
  m_detectors.clear();
}

auto
DetectorManager::Get() -> DetectorManager& {
  static DetectorManager singleton;
  return singleton;
}

void
DetectorManager::RegisterDetector(const std::string& tag, Detector* det) {
  m_detectors[tag] = det;
}

}  // namespace icsv::detector