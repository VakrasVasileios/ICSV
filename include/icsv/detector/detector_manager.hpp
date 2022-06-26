#pragma once

namespace detector {

class DetectorManager final {
public:
  static auto Get() -> DetectorManager&;

private:
  DetectorManager()                       = default;
  DetectorManager(const DetectorManager&) = delete;
  DetectorManager(DetectorManager&&)      = delete;
  ~DetectorManager();
};

}  // namespace detector
