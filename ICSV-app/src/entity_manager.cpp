#include "entity_manager.hpp"

namespace ICSVapp {

EntityManager::EntityManager() {
  m_font = Ogre::FontManager::getSingleton().getByName("IcsvFont",
                                                       "ICSV_RESOURCES");
}

EntityManager::~EntityManager() {
  clear_entities();
  m_scnMan->clearScene();
}

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

void
EntityManager::MakeBillboardSet(void) {
  m_billbset = m_scnMan->createBillboardSet();
  m_billbset->setMaterialName("BillboardMat", "ICSV_RESOURCES");
}

void
EntityManager::ClearBillboardSet(void) {
  auto size = m_billbset->getNumBillboards();
  for (int i = 0; i < size; i++) {
    auto* ptr = m_billbset->getBillboard(i);
    if (ptr != nullptr)
      delete ptr;
  }
  m_billbset->clear();
}

void
EntityManager::CreateBillboard(Ogre::Vector3 pos, const std::string& msg) {
  m_billbset->createBillboard(pos);
  m_font->putText(m_billbset, msg, 10.f);
}

auto
EntityManager::CreateIcsvEntity(DetectorReport*       rep,
                                const Ogre::Vector3f& pos,
                                const Ogre::Vector3f& scale) -> IcsvEntity* {
  IcsvEntity*        icsv_ent = new IcsvEntity();
  static std::size_t ps_c     = 0;
  static std::size_t ent_c    = 0;

  icsv_ent->m_node = m_scnMan->getRootSceneNode()->createChildSceneNode();
  icsv_ent->m_ent  = m_scnMan->createEntity("ent" + std::to_string(ent_c++),
                                           "Cube.mesh",
                                           "ICSV_RESOURCES");
  icsv_ent->m_node->attachObject(icsv_ent->m_ent);
  icsv_ent->m_node->setPosition(pos);
  icsv_ent->m_node->setScale(scale);

  // FIXME: bounding box starts to expand when particles emit
  icsv_ent->m_sfx_node = icsv_ent->m_node->createChildSceneNode();
  icsv_ent->m_sfx_node->setPosition(pos);
  icsv_ent->m_sfx_node->setScale(scale);
  icsv_ent->m_sfx
      = m_scnMan->createParticleSystem("ps" + std::to_string(ps_c++),
                                       "Examples/PurpleFountain");
  icsv_ent->m_sfx_node->attachObject(icsv_ent->m_sfx);
  icsv_ent->m_sfx->setEmitting(false);

  icsv_ent->SetDetectorReport(rep);
  m_entt_list.push_back(icsv_ent);

  return icsv_ent;
}

void
EntityManager::ClearEntities(void) {
  for (auto* ent : m_entt_list) {
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
EntityManager::GetEntityList(void) const -> const std::list<IcsvEntity*>& {
  return m_entt_list;
}

void
EntityManager::SortEnttsWith(const SortFunc& f) {
  m_entt_list.sort(f);
}

auto
EntityManager::RequestChildNode(void) -> Ogre::SceneNode* {
  return m_scnMan->getRootSceneNode()->createChildSceneNode();
}

void
EntityManager::RepositionEnttsOnAxisX(void) {
  int x = 0;
  for (auto& i : m_entt_list) {
    auto pos = i->GetPosition();
    i->SetPosition(x++, pos.y, pos.z);
  }
}

void
EntityManager::RepositionEnttsOnAxisZ(void) {
  int z = 0;
  for (auto& i : m_entt_list) {
    auto pos = i->GetPosition();
    i->SetPosition(pos.x, pos.y, z++);
  }
}

void
EntityManager::CreateGrid(Ogre::SceneNode* attach_point) {
#if (0)
  for (int i = -5000; i < 5001; i++) {
    CreateGridLine(attach_point, { -5000, 0, i }, { 5000, 0, i });
    CreateGridLine(attach_point, { i, 0, -5000 }, { i, 0, 5000 });
  }
#endif
}

void
EntityManager::CreateGridLine(Ogre::SceneNode*     attach_point,
                              const Ogre::Vector3& pos1,
                              const Ogre::Vector3& pos2) {
  static std::uint64_t count = 0;
  auto name = Ogre::String("GridLine" + std::to_string(count++));
  std::cout << "GridLine Name: " << name << '\n';
  Ogre::ManualObject* line = m_scnMan->createManualObject(name);
  line->begin("BaseWhiteNoLighting", Ogre::RenderOperation::OT_LINE_LIST);
  line->position(pos1);
  line->position(pos2);
  line->end();
  attach_point->attachObject(line);
}

}  // namespace ICSVapp