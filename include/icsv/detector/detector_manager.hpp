#pragma once

#include <functional>
#include <json/json.h>
#include <string>
#include <unordered_map>

#define REGISTER_DETECTOR(tag, det) \
  icsv::detector::DetectorManager::Get().RegisterDetector(tag, det)

namespace icsv::detector {

class Detector;

class DetectorManager final {
public:
  static auto Get() -> DetectorManager&;

  void RegisterDetector(const std::string& tag, Detector* det);

  void UseDetectors(void);

private:
  DetectorManager()                       = default;
  DetectorManager(const DetectorManager&) = delete;
  DetectorManager(DetectorManager&&)      = delete;
  ~DetectorManager();

  std::unordered_map<std::string, Detector*> m_detectors;
};

inline void
use_smell_detectors(void) {
  icsv::detector::DetectorManager::Get().UseDetectors();
}

}  // namespace icsv::detector
