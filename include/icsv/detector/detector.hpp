#pragma once

#include "arch/architecture_data.hpp"
#include "detector_manager.hpp"
#include "detector_report.hpp"
#include "icsv/detector/evaluation_center.hpp"
#include "report_center.hpp"

// TODO: Add interface documentation

using DetectorReport = icsv::detector::DetectorReport;
using SourceInfo     = icsv::detector::SourceInfo;
using ArchData       = icsv::detector::arch::ArchData;
using Detector       = icsv::detector::Detector;

namespace icsv::detector {

class Detector {
public:
  Detector(const std::string& tag = "") {
    assert(tag != "");
    REGISTER_DETECTOR(tag, this);
  }

  virtual ~Detector() = default;

  virtual void DetectSmell(const ArchData& data) = 0;
};

}  // namespace icsv::detector
