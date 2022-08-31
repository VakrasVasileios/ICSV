#pragma once

#include "detector_manager.hpp"
#include "detector_report.hpp"
#include "report_center.hpp"
#include <iostream>
#include <json/json.h>
#include <string>

// TODO: Add interface documentation

using DetectorReport = icsv::detector::DetectorReport;

namespace icsv::detector {

class Detector {
public:
  Detector(const std::string& tag) { REGISTER_DETECTOR(tag, this); }
  virtual ~Detector() = default;

  virtual void SmellDetect(const Json::Value& arch) = 0;
};

}  // namespace icsv::detector
