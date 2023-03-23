#pragma once

#include "icsv_entity.hpp"
#include <Ogre.h>
#include <OgreManualObject.h>

namespace ICSVapp {

class EntityManager final {
public:
  using Pred = std::function<bool(IcsvEntity*)>;

public:
  static auto Get(void) -> EntityManager&;

  void SetSceneManager(Ogre::SceneManager* scnMan);
  auto CreateIcsvEntity(DetectorReport* rep) -> IcsvEntity*;
  auto FindEntityIf(const Pred& pred) const -> IcsvEntity*;
  auto GetEntityList(void) const -> const std::list<IcsvEntity*>&;

private:
  Ogre::SceneManager*    m_scnMan{ nullptr };
  std::list<IcsvEntity*> m_entt_list;

  EntityManager()                     = default;
  EntityManager(const EntityManager&) = delete;
  EntityManager(EntityManager&&)      = delete;
  ~EntityManager()                    = default;

  auto RequestChildNode(void) -> Ogre::SceneNode*;

  Ogre::ManualObject* CreateCubeMesh(void);
};

inline void
set_scene_manager(Ogre::SceneManager* scnMan) {
  EntityManager::Get().SetSceneManager(scnMan);
}

inline auto
create_icsv_entity(DetectorReport* rep) -> IcsvEntity* {
  return EntityManager::Get().CreateIcsvEntity(rep);
}

inline auto
find_entt_if(const EntityManager::Pred& pred) -> IcsvEntity* {
  return EntityManager::Get().FindEntityIf(pred);
}

inline auto
get_entity_list(void) -> const std::list<IcsvEntity*>& {
  return EntityManager::Get().GetEntityList();
}

}  // namespace ICSVapp