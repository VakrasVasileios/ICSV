#include "detector/detector.hpp"
#include "detector/detector_manager.hpp"

namespace detector {

Detector::Detector() { DetectorManager::Get().RegisterDetector(this); }

}  // namespace detector
