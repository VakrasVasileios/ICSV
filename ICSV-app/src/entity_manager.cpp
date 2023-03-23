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
EntityManager::CreateIcsvEntity(DetectorReport*       rep,
                                const Ogre::Vector3f& pos,
                                const Ogre::Vector3f& scale) -> IcsvEntity* {
  IcsvEntity* icsv_ent = new IcsvEntity();
  // icsv_ent->m_ent      = m_scnMan->createEntity(mesh_name);

  icsv_ent->m_node = m_scnMan->getRootSceneNode()->createChildSceneNode();
  icsv_ent->m_ent  = CreateCubeMesh(pos, scale);
  // std::cout << "Bounding box " << manObj->getBoundingBox() << std::endl;
  icsv_ent->m_node->attachObject(icsv_ent->m_ent);
  icsv_ent->SetDetectorReport(rep);
  m_entt_list.push_back(icsv_ent);
  return icsv_ent;
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

auto
EntityManager::RequestChildNode(void) -> Ogre::SceneNode* {
  return m_scnMan->getRootSceneNode()->createChildSceneNode();
}

auto
EntityManager::CreateCubeMesh(const Ogre::Vector3f& pos,
                              const Ogre::Vector3f& scale)
    -> Ogre::ManualObject* {

  Ogre::ManualObject* cube = new Ogre::ManualObject("Cube");
  cube->begin("BaseWhiteNoLighting", Ogre::RenderOperation::OT_TRIANGLE_LIST);

  cube->position(pos.x + scale.x / 2, pos.y - scale.y / 2, pos.z + scale.z / 2);
  cube->normal(0.408248, -0.816497, 0.408248);
  cube->textureCoord(1, 0);
  cube->position(pos.x - scale.x / 2, pos.y - scale.y / 2, pos.z - scale.z / 2);
  cube->normal(-0.408248, -0.816497, -0.408248);
  cube->textureCoord(0, 1);
  cube->position(pos.x + scale.x / 2, pos.y - scale.y / 2, pos.z - scale.z / 2);
  cube->normal(0.666667, -0.333333, -0.666667);
  cube->textureCoord(1, 1);
  cube->position(pos.x - scale.x / 2, pos.y - scale.y / 2, pos.z + scale.z / 2);
  cube->normal(-0.666667, -0.333333, 0.666667);
  cube->textureCoord(0, 0);
  cube->position(pos.x + scale.x / 2, pos.y + scale.y / 2, pos.z + scale.z / 2);
  cube->normal(0.666667, 0.333333, 0.666667);
  cube->textureCoord(1, 0);
  cube->position(pos.x - scale.x / 2, pos.y - scale.y / 2, pos.z + scale.z / 2);
  cube->normal(-0.666667, -0.333333, 0.666667);
  cube->textureCoord(0, 1);
  cube->position(pos.x + scale.x / 2, pos.y - scale.y / 2, pos.z + scale.z / 2);
  cube->normal(0.408248, -0.816497, 0.408248);
  cube->textureCoord(1, 1);
  cube->position(pos.x - scale.x / 2, pos.y + scale.y / 2, pos.z + scale.z / 2);
  cube->normal(-0.408248, 0.816497, 0.408248);
  cube->textureCoord(0, 0);
  cube->position(pos.x - scale.x / 2, pos.y + scale.y / 2, pos.z - scale.z / 2);
  cube->normal(-0.666667, 0.333333, -0.666667);
  cube->textureCoord(0, 1);
  cube->position(pos.x - scale.x / 2, pos.y - scale.y / 2, pos.z - scale.z / 2);
  cube->normal(-0.408248, -0.816497, -0.408248);
  cube->textureCoord(1, 1);
  cube->position(pos.x - scale.x / 2, pos.y - scale.y / 2, pos.z + scale.z / 2);
  cube->normal(-0.666667, -0.333333, 0.666667);
  cube->textureCoord(1, 0);
  cube->position(pos.x + scale.x / 2, pos.y - scale.y / 2, pos.z - scale.z / 2);
  cube->normal(0.666667, -0.333333, -0.666667);
  cube->textureCoord(0, 1);
  cube->position(pos.x + scale.x / 2, pos.y + scale.y / 2, pos.z - scale.z / 2);
  cube->normal(0.408248, 0.816497, -0.408248);
  cube->textureCoord(1, 1);
  cube->position(pos.x + scale.x / 2, pos.y - scale.y / 2, pos.z + scale.z / 2);
  cube->normal(0.408248, -0.816497, 0.408248);
  cube->textureCoord(0, 0);
  cube->position(pos.x + scale.x / 2, pos.y - scale.y / 2, pos.z - scale.z / 2);
  cube->normal(0.666667, -0.333333, -0.666667);
  cube->textureCoord(1, 0);
  cube->position(pos.x - scale.x / 2, pos.y - scale.y / 2, pos.z - scale.z / 2);
  cube->normal(-0.408248, -0.816497, -0.408248);
  cube->textureCoord(0, 0);
  cube->position(pos.x - scale.x / 2, pos.y + scale.y / 2, pos.z + scale.z / 2);
  cube->normal(-0.408248, 0.816497, 0.408248);
  cube->textureCoord(1, 0);
  cube->position(pos.x + scale.x / 2, pos.y + scale.y / 2, pos.z - scale.z / 2);
  cube->normal(0.408248, 0.816497, -0.408248);
  cube->textureCoord(0, 1);
  cube->position(pos.x - scale.x / 2, pos.y + scale.y / 2, pos.z - scale.z / 2);
  cube->normal(-0.666667, 0.333333, -0.666667);
  cube->textureCoord(1, 1);
  cube->position(pos.x + scale.x / 2, pos.y + scale.y / 2, pos.z + scale.z / 2);
  cube->normal(0.666667, 0.333333, 0.666667);
  cube->textureCoord(0, 0);

  cube->triangle(0, 1, 2);
  cube->triangle(3, 1, 0);
  cube->triangle(4, 5, 6);
  cube->triangle(4, 7, 5);
  cube->triangle(8, 9, 10);
  cube->triangle(10, 7, 8);
  cube->triangle(4, 11, 12);
  cube->triangle(4, 13, 11);
  cube->triangle(14, 8, 12);
  cube->triangle(14, 15, 8);
  cube->triangle(16, 17, 18);
  cube->triangle(16, 19, 17);
  cube->end();

  return cube;
}

}  // namespace ICSVapp