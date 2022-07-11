#pragma once

#include "detector_manager.hpp"
#include <iostream>
#include <string>

// TODO: Add interface documentation
namespace detector {

struct DetectorReport;

#define REGISTER_DETECTOR(tag, det) \
  DetectorManager::Get().RegisterDetector(tag, det)

#define DETECTOR [](const std::string& input) -> DetectorReport

}  // namespace detector
