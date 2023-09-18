#include "icsv_entity.hpp"
#include "entity_manager.hpp"

namespace ICSVapp {

IcsvEntity::~IcsvEntity() {
  m_sfx->setEmitting(false);
  m_node->showBoundingBox(false);
  m_report = nullptr;
  m_node->detachAllObjects();
  m_sfx_node->detachAllObjects();
  destroy_entity(m_ent);
  destroy_particle_system(m_sfx);
  destroy_node(m_node);
  destroy_node(m_sfx_node);
}

void
IcsvEntity::AdjustScaleBy(double x, double y, double z) {
  m_node->setScale(m_node->getScale() + Ogre::Vector3(x, y, z));
}

void
IcsvEntity::SetPosition(double x, double y, double z) {
  m_node->setPosition(x, y, z);
  m_sfx_node->setPosition(x, y, z);
}

auto
IcsvEntity::GetPosition(void) -> Ogre::Vector3f {
  return m_node->getPosition();
}

void
IcsvEntity::SetScale(double x, double y, double z) {
  m_node->setScale(x, y, z);
}

auto
IcsvEntity::GetScale(void) -> Ogre::Vector3f {
  return m_node->getScale();
}

}  // namespace ICSVapp