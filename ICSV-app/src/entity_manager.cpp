#include "entity_manager.hpp"
#include "icsv_entity.hpp"

namespace ICSVapp {

auto
EntityManager::Get(void) -> EntityManager& {
  static EntityManager singleton;
  return singleton;
}

void
EntityManager::SetSceneManager(Ogre::SceneManager* scnMan) {
  assert(m_scnMan == nullptr);
  m_scnMan = scnMan;
}
auto
EntityManager::RequestChildNode(void) -> Ogre::SceneNode* {
  return m_scnMan->getRootSceneNode()->createChildSceneNode();
}

auto
EntityManager::CreateIcsvEntity(const char* mesh_name) -> IcsvEntity* {
  IcsvEntity* ent = (IcsvEntity*) m_scnMan->createEntity(mesh_name);
  ent->m_node     = m_scnMan->getRootSceneNode()->createChildSceneNode();
  ent->m_node->attachObject(ent);
  m_entt_list.push_back(ent);
  return ent;
}

}  // namespace ICSVapp