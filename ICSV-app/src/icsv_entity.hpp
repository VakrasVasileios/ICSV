#pragma once

#include <Ogre.h>
#include <OgreParticleSystem.h>
#include "icsv/detector/detector_report.hpp"

namespace ICSVapp {

using DetectorReport = icsv::detector::DetectorReport;

class IcsvEntity {
public:
  ~IcsvEntity() = default;

  void AdjustScaleBy(double x, double y, double z);

  auto GetDetectorReport(void) const -> DetectorReport* { return m_report; }
  void SetDetectorReport(DetectorReport* rep) { m_report = rep; }

  auto GetBoundingBox(void) -> const Ogre::AxisAlignedBox& {
    return m_ent->getBoundingBox();
  }
  void SetBoundingBox(const Ogre::AxisAlignedBox& box) {
    m_ent->setBoundingBox(box);
  }
  void ShowBoundingBox(bool show) { m_node->showBoundingBox(show); }

  void SetPosition(double x, double y, double z);
  auto GetPosition(void) -> Ogre::Vector3f;

  void SetScale(double x, double y, double z);
  auto GetScale(void) -> Ogre::Vector3f;

  void Emmision(bool e) { m_sfx->setEmitting(e); }

  void FlipVisibility(void) { m_node->flipVisibility(false); }

  friend class EntityManager;

protected:
  IcsvEntity() = default;

private:
  Ogre::SceneNode*      m_node;
  Ogre::ManualObject*   m_ent;
  Ogre::ParticleSystem* m_sfx;
  Ogre::SceneNode*      m_sfx_node;
  DetectorReport*       m_report{ nullptr };
};

}  // namespace ICSVapp