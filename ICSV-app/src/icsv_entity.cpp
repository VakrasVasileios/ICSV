#include "icsv_entity.hpp"
#include "entity_manager.hpp"

namespace ICSVapp {

IcsvEntity::~IcsvEntity() {
  m_node->showBoundingBox(false);
  m_report = nullptr;
  m_node->detachObject(m_ent);
  destroy_entity(m_ent);
  destroy_node(m_node);
}

void
IcsvEntity::AdjustScaleBy(double x, double y, double z) {
  m_node->setScale(m_node->getScale() + Ogre::Vector3(x, y, z));
}

void
IcsvEntity::SetPosition(double x, double y, double z) {
  m_node->setPosition(x, y, z);
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