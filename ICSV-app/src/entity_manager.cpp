#include "entity_manager.hpp"
#include "material_factory.hpp"

namespace ICSVapp {

EntityManager::EntityManager() {
  m_font = Ogre::FontManager::getSingleton().getByName("IcsvFont",
                                                       "ICSV_RESOURCES");
}

EntityManager::~EntityManager() {}

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
EntityManager::CreateIcsvEntity(DetectorReport*       rep,
                                const Ogre::Vector3f& pos,
                                const Ogre::Vector3f& scale) -> IcsvEntity* {
  IcsvEntity*        icsv_ent = new IcsvEntity();
  static std::size_t ent_c    = 0;

  icsv_ent->m_node = m_scnMan->getRootSceneNode()->createChildSceneNode();
  icsv_ent->m_ent  = m_scnMan->createEntity("ent" + std::to_string(ent_c++),
                                           "Cube.mesh",
                                           "ICSV_RESOURCES");

  auto mat = MaterialFactory::Get().GetMaterialFor(rep->smell_tag);
  assert(mat.operator bool());
  icsv_ent->m_ent->setMaterial(mat->m_material);

  icsv_ent->m_node->attachObject(icsv_ent->m_ent);
  icsv_ent->m_node->setPosition(pos);
  icsv_ent->m_node->setScale(scale);

  icsv_ent->SetDetectorReport(rep);

  m_entt_list.push_back(icsv_ent);

  return icsv_ent;
}

auto
EntityManager::CreateMovableText(const std::string& caption,
                                 Ogre::SceneNode*   attach_point)
    -> MovableTextPtr {
  static std::uint64_t count = 0;

  auto name   = Ogre::String("MvTxt" + std::to_string(count));
  auto mv_txt = std::make_shared<Ogre::MovableText>(name,
                                                    caption,
                                                    "Sans",
                                                    m_font_size,
                                                    Ogre::ColourValue::White,
                                                    "ICSV_RESOURCES");
  m_graph_tags.push_back(mv_txt);
  mv_txt->setSpaceWidth(0);
  mv_txt->showOnTop(true);
  mv_txt->setRenderQueueGroup(Ogre::RenderQueueGroupID::RENDER_QUEUE_4);
  mv_txt->setVisibilityFlags(0x0002);
  mv_txt->setTextAlignment(Ogre::MovableText::H_CENTER,
                           Ogre::MovableText::V_CENTER);
  attach_point->attachObject(mv_txt.get());

  return mv_txt;
}

void
EntityManager::ClearEntities(void) {
  for (auto* ent : m_entt_list) {
    ent->ShowBoundingBox(false);
    delete ent;
    ent = nullptr;
  }
  m_entt_list.clear();
}

auto
EntityManager::FindEntityIf(const Pred& pred) const -> IcsvEntity* {
  for (auto ref : m_entt_list)
    if (pred(ref))
      return ref;
  return nullptr;
}

auto
EntityManager::GetEntityList(void) const -> const EntityList& {
  return m_entt_list;
}

void
EntityManager::SortEnttsWith(const SortFunc& f) {
  std::stable_sort(m_entt_list.begin(), m_entt_list.end(), f);
}

auto
EntityManager::RequestChildNode(void) -> Ogre::SceneNode* {
  return m_scnMan->getRootSceneNode()->createChildSceneNode();
}

void
EntityManager::CreateGrid(Ogre::SceneNode*) {
#if (0)
  for (int i = -5000; i < 5001; i++) {
    CreateGridLine(attach_point, { -5000, 0, i }, { 5000, 0, i });
    CreateGridLine(attach_point, { i, 0, -5000 }, { i, 0, 5000 });
  }
#endif
}

auto
EntityManager::CreateGridLine(Ogre::SceneNode*     attach_point,
                              const Ogre::Vector3& pos1,
                              const Ogre::Vector3& pos2)
    -> Ogre::ManualObject* {
  static std::uint64_t count = 0;

  auto name = Ogre::String("GridLine" + std::to_string(count++));

  Ogre::ManualObject* line = m_scnMan->createManualObject(name);

  line->begin("BaseWhiteNoLighting", Ogre::RenderOperation::OT_LINE_LIST);
  line->position(pos1);
  line->position(pos2);
  line->end();

  attach_point->attachObject(line);

  return line;
}

}  // namespace ICSVapp