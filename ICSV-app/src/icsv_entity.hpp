#pragma once

#include <Ogre.h>
#include "icsv/detector/detector_report.hpp"

namespace ICSVapp {

using DetectorReport = icsv::detector::DetectorReport;

class IcsvEntity : public Ogre::Entity {
public:
  IcsvEntity() = default;

  void AdjustScaleBy(double x, double y, double z);
  void SetDetectorReport(DetectorReport* rep) { m_report = rep; }
  void SetPosition(double x, double y, double z);
  void SetScale(double x, double y, double z);
  auto GetDetectorReport() const -> DetectorReport* { return m_report; }

  friend class EntityManager;

private:
  Ogre::SceneNode* m_node;
  DetectorReport*  m_report{ nullptr };
};

}  // namespace ICSVapp