#include "icsv_entity.hpp"
#include <imgui.h>

namespace ICSVapp {

void
IcsvEntity::AdjustScaleBy(double x, double y, double z) {
  m_node->setScale(m_node->getScale() + Ogre::Vector3(x, y, z));
}

void
IcsvEntity::SetPosition(double x, double y, double z) {
  m_node->setPosition(x, y, z);
}

void
IcsvEntity::SetScale(double x, double y, double z) {
  m_node->setScale(x, y, z);
}

}  // namespace ICSVapp