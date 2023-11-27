#pragma once

#include <Ogre.h>
#include <OgreParticleSystem.h>
#include "icsv/detector/detector_report.hpp"

namespace ICSVapp {

using DetectorReport = icsv::detector::DetectorReport;

class IcsvEntity {
public:
  ~IcsvEntity();

  void AdjustScaleBy(double x, double y, double z);

  auto GetDetectorReport(void) -> DetectorReport& { return *m_report; }
  auto GetDetectorReport(void) const -> const DetectorReport& {
    return *m_report;
  }
  void SetDetectorReport(DetectorReport* rep) { m_report = rep; }

  auto GetBoundingBox(void) -> const Ogre::AxisAlignedBox& {
    return m_ent->getWorldBoundingBox();
  }

  void ShowBoundingBox(bool show) { m_node->showBoundingBox(show); }

  void SetPosition(double x, double y, double z);
  auto GetPosition(void) -> Ogre::Vector3f;

  void SetScale(double x, double y, double z);
  auto GetScale(void) -> Ogre::Vector3f;

  auto GetMaterial(void) -> Ogre::MaterialPtr;
  void SetMaterial(Ogre::MaterialPtr);

  void FlipVisibility(void) { m_node->flipVisibility(false); }

  friend class EntityManager;

protected:
  IcsvEntity() = default;

private:
  Ogre::SceneNode* m_node;
  Ogre::Entity*    m_ent;
  DetectorReport*  m_report{ nullptr };
};

}  // namespace ICSVapp