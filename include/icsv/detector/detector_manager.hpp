#pragma once

#include <list>

namespace detector {

class Detector;

class DetectorManager final {
public:
  static auto Get() -> DetectorManager&;

  void RegisterDetector(Detector*);

private:
  DetectorManager()                       = default;
  DetectorManager(const DetectorManager&) = delete;
  DetectorManager(DetectorManager&&)      = delete;
  ~DetectorManager();

  std::list<Detector*> m_detectors;
};

}  // namespace detector
