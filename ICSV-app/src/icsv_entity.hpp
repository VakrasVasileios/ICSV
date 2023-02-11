#pragma once

#include <Ogre.h>
#include "icsv/detector/detector_report.hpp"

namespace ICSVapp {

using DetectorReport = icsv::detector::DetectorReport;

class IcsvEntity {
public:
  ~IcsvEntity() = default;

  void AdjustScaleBy(double x, double y, double z);

  auto GetDetectorReport() const -> DetectorReport* { return m_report; }
  void SetDetectorReport(DetectorReport* rep) { m_report = rep; }

  void SetPosition(double x, double y, double z);
  auto GetPosition(void) -> Ogre::Vector3f;

  void SetScale(double x, double y, double z);
  auto GetScale(void) -> Ogre::Vector3f;

  friend class EntityManager;

protected:
  IcsvEntity() = default;

private:
  Ogre::SceneNode* m_node;
  Ogre::Entity*    m_ent;
  DetectorReport*  m_report{ nullptr };
};

}  // namespace ICSVapp