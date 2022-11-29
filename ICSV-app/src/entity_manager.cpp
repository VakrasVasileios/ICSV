#include "entity_manager.hpp"

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
EntityManager::CreateIcsvEntity(const char* mesh_name, DetectorReport* rep)
    -> IcsvEntity* {
  IcsvEntity* ent = (IcsvEntity*) m_scnMan->createEntity(mesh_name);
  ent->m_node     = m_scnMan->getRootSceneNode()->createChildSceneNode();
  ent->m_node->attachObject(ent);
  ent->SetDetectorReport(rep);
  m_entt_list.push_back(ent);
  return ent;
}

auto
EntityManager::FindEntityIf(const Pred& pred) const -> IcsvEntity* {
  for (auto ref : m_entt_list)
    if (pred(ref))
      return ref;
  return nullptr;
}

auto
EntityManager::RequestChildNode(void) -> Ogre::SceneNode* {
  return m_scnMan->getRootSceneNode()->createChildSceneNode();
}

}  // namespace ICSVapp