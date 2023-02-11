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
  IcsvEntity* icsv_ent = new IcsvEntity();
  // icsv_ent->m_ent      = m_scnMan->createEntity(mesh_name);
  icsv_ent->m_node = m_scnMan->getRootSceneNode()->createChildSceneNode();
  icsv_ent->m_node->attachObject(
      /*icsv_ent->m_ent*/ CreateCubeMesh("Cube", "Glass"));  // FIXME: Add
                                                             // some material
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
EntityManager::RequestChildNode(void) -> Ogre::SceneNode* {
  return m_scnMan->getRootSceneNode()->createChildSceneNode();
}

Ogre::ManualObject*
EntityManager::CreateCubeMesh(Ogre::String name, Ogre::String matName) {

  Ogre::ManualObject* cube = new Ogre::ManualObject(name);
  cube->begin(matName);

  cube->position(0.5, -0.5, 1.0);
  cube->normal(0.408248, -0.816497, 0.408248);
  cube->textureCoord(1, 0);
  cube->position(-0.5, -0.5, 0.0);
  cube->normal(-0.408248, -0.816497, -0.408248);
  cube->textureCoord(0, 1);
  cube->position(0.5, -0.5, 0.0);
  cube->normal(0.666667, -0.333333, -0.666667);
  cube->textureCoord(1, 1);
  cube->position(-0.5, -0.5, 1.0);
  cube->normal(-0.666667, -0.333333, 0.666667);
  cube->textureCoord(0, 0);
  cube->position(0.5, 0.5, 1.0);
  cube->normal(0.666667, 0.333333, 0.666667);
  cube->textureCoord(1, 0);
  cube->position(-0.5, -0.5, 1.0);
  cube->normal(-0.666667, -0.333333, 0.666667);
  cube->textureCoord(0, 1);
  cube->position(0.5, -0.5, 1.0);
  cube->normal(0.408248, -0.816497, 0.408248);
  cube->textureCoord(1, 1);
  cube->position(-0.5, 0.5, 1.0);
  cube->normal(-0.408248, 0.816497, 0.408248);
  cube->textureCoord(0, 0);
  cube->position(-0.5, 0.5, 0.0);
  cube->normal(-0.666667, 0.333333, -0.666667);
  cube->textureCoord(0, 1);
  cube->position(-0.5, -0.5, 0.0);
  cube->normal(-0.408248, -0.816497, -0.408248);
  cube->textureCoord(1, 1);
  cube->position(-0.5, -0.5, 1.0);
  cube->normal(-0.666667, -0.333333, 0.666667);
  cube->textureCoord(1, 0);
  cube->position(0.5, -0.5, 0.0);
  cube->normal(0.666667, -0.333333, -0.666667);
  cube->textureCoord(0, 1);
  cube->position(0.5, 0.5, 0.0);
  cube->normal(0.408248, 0.816497, -0.408248);
  cube->textureCoord(1, 1);
  cube->position(0.5, -0.5, 1.0);
  cube->normal(0.408248, -0.816497, 0.408248);
  cube->textureCoord(0, 0);
  cube->position(0.5, -0.5, 0.0);
  cube->normal(0.666667, -0.333333, -0.666667);
  cube->textureCoord(1, 0);
  cube->position(-0.5, -0.5, 0.0);
  cube->normal(-0.408248, -0.816497, -0.408248);
  cube->textureCoord(0, 0);
  cube->position(-0.5, 0.5, 1.0);
  cube->normal(-0.408248, 0.816497, 0.408248);
  cube->textureCoord(1, 0);
  cube->position(0.5, 0.5, 0.0);
  cube->normal(0.408248, 0.816497, -0.408248);
  cube->textureCoord(0, 1);
  cube->position(-0.5, 0.5, 0.0);
  cube->normal(-0.666667, 0.333333, -0.666667);
  cube->textureCoord(1, 1);
  cube->position(0.5, 0.5, 1.0);
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