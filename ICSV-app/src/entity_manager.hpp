#pragma once

#include "icsv_entity.hpp"
#include <Ogre.h>
#include <OgreBillboardSet.h>
#include <OgreManualObject.h>
#include <OgreFontManager.h>

namespace ICSVapp {

class EntityManager final {
public:
  using Pred = std::function<bool(IcsvEntity*)>;

public:
  static auto Get(void) -> EntityManager&;

  void SetSceneManager(Ogre::SceneManager* scnMan);
  void MakeBillboardSet(void);
  void ClearBillboardSet(void);
  void CreateBillboard(Ogre::Vector3 pos, const std::string& msg);
  auto CreateIcsvEntity(DetectorReport*       rep,
                        const Ogre::Vector3f& pos,
                        const Ogre::Vector3f& scale) -> IcsvEntity*;
  auto FindEntityIf(const Pred& pred) const -> IcsvEntity*;
  auto GetEntityList(void) const -> const std::list<IcsvEntity*>&;

private:
  Ogre::SceneManager*    m_scnMan{ nullptr };
  Ogre::BillboardSet*    m_billbset{ nullptr };
  std::list<IcsvEntity*> m_entt_list;
  Ogre::FontPtr          m_font;

  EntityManager();
  EntityManager(const EntityManager&) = delete;
  EntityManager(EntityManager&&)      = delete;
  ~EntityManager()                    = default;

  auto RequestChildNode(void) -> Ogre::SceneNode*;

  Ogre::ManualObject* CreateCubeMesh(const Ogre::Vector3f& pos,
                                     const Ogre::Vector3f& scale);
};

inline void
set_scene_manager(Ogre::SceneManager* scnMan) {
  EntityManager::Get().SetSceneManager(scnMan);
}

inline auto
create_icsv_entity(DetectorReport*       rep,
                   const Ogre::Vector3f& pos,
                   const Ogre::Vector3f& scale) -> IcsvEntity* {
  return EntityManager::Get().CreateIcsvEntity(rep, pos, scale);
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