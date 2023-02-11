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
  auto CreateIcsvEntity(const char* mesh_name, DetectorReport* rep)
      -> IcsvEntity*;
  auto FindEntityIf(const Pred& pred) const -> IcsvEntity*;

private:
  Ogre::SceneManager*    m_scnMan{ nullptr };
  std::list<IcsvEntity*> m_entt_list;

  EntityManager()                     = default;
  EntityManager(const EntityManager&) = delete;
  EntityManager(EntityManager&&)      = delete;
  ~EntityManager()                    = default;

  auto RequestChildNode(void) -> Ogre::SceneNode*;

  Ogre::ManualObject* CreateCubeMesh(Ogre::String name, Ogre::String matName);
};

inline void
set_scene_manager(Ogre::SceneManager* scnMan) {
  EntityManager::Get().SetSceneManager(scnMan);
}

inline auto
create_icsv_entity(const char* mesh_name, DetectorReport* rep) -> IcsvEntity* {
  return EntityManager::Get().CreateIcsvEntity(mesh_name, rep);
}

inline auto
find_entt_if(const EntityManager::Pred& pred) -> IcsvEntity* {
  return EntityManager::Get().FindEntityIf(pred);
}

}  // namespace ICSVapp