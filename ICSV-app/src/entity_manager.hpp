#pragma once

#include <Ogre.h>

namespace ICSVapp {

class IcsvEntity;

class EntityManager final {
public:
  static auto Get(void) -> EntityManager&;

  void SetSceneManager(Ogre::SceneManager* scnMan);
  auto CreateIcsvEntity(const char* mesh_name) -> IcsvEntity*;

private:
  Ogre::SceneManager* m_scnMan{ nullptr };

  EntityManager()                     = default;
  EntityManager(const EntityManager&) = delete;
  EntityManager(EntityManager&&)      = delete;
  ~EntityManager()                    = default;

  auto RequestChildNode(void) -> Ogre::SceneNode*;
};

inline void
set_scene_manager(Ogre::SceneManager* scnMan) {
  EntityManager::Get().SetSceneManager(scnMan);
}

inline auto
create_icsv_entity(const char* mesh_name) -> IcsvEntity* {
  return EntityManager::Get().CreateIcsvEntity(mesh_name);
}

}  // namespace ICSVapp