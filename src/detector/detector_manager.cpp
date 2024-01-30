#include "icsv/detector/detector_manager.hpp"
#include "icsv/detector/arch/architecture_holder.hpp"
#include "icsv/detector/detector.hpp"

namespace icsv::detector {

DetectorManager::~DetectorManager() {
  for (auto& d : m_detectors)
    delete d.second;
  m_detectors.clear();
}

auto
DetectorManager::Get() -> DetectorManager& {
  static DetectorManager singleton;
  return singleton;
}

auto
DetectorManager::GetAllDetectorTags(void) -> std::list<std::string> {
  auto ret = std::list<std::string>();

  for (auto& d : m_detectors)
    ret.push_back(d.first);

  return ret;
}

auto
DetectorManager::GetDetectors(void) const
    -> const std::map<std::string, Detector*>& {
  return m_detectors;
}

void
DetectorManager::RegisterDetector(const std::string& tag, Detector* det) {
  if (m_detectors.contains(tag))
    std::cout << "Newly registered detector with tag \"" << tag
              << "\" overwrites existing one!\n";

  m_detectors[tag] = det;
}

void
DetectorManager::UseDetectors(void) {
  for (auto& d : m_detectors) {
    if (d.second->IsActive())
      d.second->DetectSmell(arch::ArchitectureHolder::Get().GetArchitecture());
  }
}

void
DetectorManager::UseDetectorWithTag(const std::string& tag) {
  if (m_detectors.contains(tag))
    m_detectors[tag]->DetectSmell(
        arch::ArchitectureHolder::Get().GetArchitecture());
}

}  // namespace icsv::detector