#pragma once

#include "icsv_entity.hpp"
#include <Ogre.h>
#include <OgreBillboardSet.h>
#include <OgreManualObject.h>
#include <OgreFontManager.h>
#include <memory>
#include "movable_text.hpp"

namespace ICSVapp {

class Chart;

using MovableTextPtr = std::shared_ptr<Ogre::MovableText>;

class EntityManager final {
public:
  using Pred       = std::function<bool(IcsvEntity*)>;
  using SortFunc   = std::function<bool(IcsvEntity*, IcsvEntity*)>;
  using EntityList = std::vector<IcsvEntity*>;

public:
  static auto Get(void) -> EntityManager&;

  void SetSceneManager(Ogre::SceneManager* scnMan);
  auto CreateIcsvEntity(DetectorReport*       rep,
                        const Ogre::Vector3f& pos,
                        const Ogre::Vector3f& scale) -> IcsvEntity*;
  auto CreateMovableText(const std::string& caption,
                         Ogre::SceneNode*   attach_point) -> MovableTextPtr;
  void CreateGrid(Ogre::SceneNode* attach_point);

  void ClearEntities(void);
  void ClearGraphTags(void) { m_graph_tags.clear(); }
  void ClearScene(void) { m_scnMan->clearScene(); }

  void DestroyEntity(Ogre::Entity* e) { m_scnMan->destroyEntity(e); }
  void DestroyMovableTxt(Ogre::MovableText* mt) {
    m_scnMan->destroyMovableObject(mt);
  }
  void DestroyNode(Ogre::SceneNode* n) { m_scnMan->destroySceneNode(n); }
  void DestroyParticleSystem(Ogre::ParticleSystem* ps) {
    m_scnMan->destroyParticleSystem(ps);
  }

  auto FindEntityIf(const Pred& pred) const -> IcsvEntity*;

  auto GetEntityList(void) const -> const EntityList&;
  void SortEnttsWith(const SortFunc& f);

  friend class Chart;

private:
  Ogre::SceneManager* m_scnMan{ nullptr };
  EntityList          m_entt_list;

  std::list<MovableTextPtr> m_graph_tags;
  Ogre::FontPtr             m_font;
  int                       m_font_size = 2;

  EntityManager();
  EntityManager(const EntityManager&) = delete;
  EntityManager(EntityManager&&)      = delete;
  ~EntityManager();

  void CreateGridLine(Ogre::SceneNode*     attach_point,
                      const Ogre::Vector3& pos1,
                      const Ogre::Vector3& pos2);
  auto RequestChildNode(void) -> Ogre::SceneNode*;
};

inline void
set_scene_manager(Ogre::SceneManager* scnMan) {
  EntityManager::Get().SetSceneManager(scnMan);
}

inline void
create_grid(Ogre::SceneNode* attach_point) {
  EntityManager::Get().CreateGrid(attach_point);
}

inline auto
create_icsv_entity(DetectorReport*       rep,
                   const Ogre::Vector3f& pos,
                   const Ogre::Vector3f& scale) -> IcsvEntity* {
  return EntityManager::Get().CreateIcsvEntity(rep, pos, scale);
}

inline void
clear_entities(void) {
  EntityManager::Get().ClearEntities();
}

inline void
clear_graph_tags(void) {
  EntityManager::Get().ClearGraphTags();
}

inline void
clear_scene(void) {
  EntityManager::Get().ClearScene();
}

inline void
destroy_entity(Ogre::Entity* e) {
  EntityManager::Get().DestroyEntity(e);
}

inline void
destroy_movable_txt(Ogre::MovableText* mt) {
  EntityManager::Get().DestroyMovableTxt(mt);
}

inline void
destroy_node(Ogre::SceneNode* n) {
  EntityManager::Get().DestroyNode(n);
}

inline void
destroy_particle_system(Ogre::ParticleSystem* ps) {
  EntityManager::Get().DestroyParticleSystem(ps);
}

inline auto
find_entt_if(const EntityManager::Pred& pred) -> IcsvEntity* {
  return EntityManager::Get().FindEntityIf(pred);
}

inline auto
get_entity_list(void) -> const EntityManager::EntityList& {
  return EntityManager::Get().GetEntityList();
}

}  // namespace ICSVapp