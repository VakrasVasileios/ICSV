#pragma once

#include <functional>
#include <string>
#include <unordered_map>

namespace detector {

struct DetectorReport;

class DetectorManager final {
public:
  using Detector = std::function<DetectorReport(const std::string&)>;

public:
  static auto Get() -> DetectorManager&;

  void RegisterDetector(const std::string& tag, const Detector& det);

private:
  DetectorManager()                       = default;
  DetectorManager(const DetectorManager&) = delete;
  DetectorManager(DetectorManager&&)      = delete;
  ~DetectorManager();

  std::unordered_map<std::string, Detector> m_detectors;
};

}  // namespace detector
