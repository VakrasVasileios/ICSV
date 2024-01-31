#pragma once

#include "arch/architecture_data.hpp"
#include "detector_manager.hpp"
#include "detector_report.hpp"
#include "icsv/detector/evaluation_center.hpp"
#include "report_center.hpp"

using DetectorReport = icsv::detector::DetectorReport;
using SourceInfo     = icsv::detector::SourceInfo;
using ArchData       = icsv::detector::arch::ArchData;
using Detector       = icsv::detector::Detector;

namespace icsv::detector {

class Detector {
public:
  Detector(const std::string& tag = "") : m_tag(tag) {
    assert(tag != "");
    REGISTER_DETECTOR(tag, this);
  }

  virtual ~Detector() = default;

  virtual void DetectSmell(const ArchData& data) = 0;

  bool IsActive(void) { return m_is_active; }
  auto GetActivePtr(void) -> bool* { return &m_is_active; }
  void SetActive(bool active) { m_is_active = active; }

protected:
  virtual auto GetEvaluator(void) -> SmellEvaluator*;

private:
  const std::string m_tag;
  bool              m_is_active = true;
};

}  // namespace icsv::detector
