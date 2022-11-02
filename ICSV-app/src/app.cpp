#include "app.hpp"
#include "gui/gui.hpp"
#include "icsv_entity.hpp"
#include "entity_manager.hpp"
#include <SDL2/SDL.h>

namespace ICSVapp {

bool
ICSVapp::frameStarted(const Ogre::FrameEvent& evt) {
  OgreBites::ApplicationContext::frameStarted(evt);

  Ogre::ImGuiOverlay::NewFrame();

  ImGui::ShowDemoWindow();
  show_menu();

  return true;
}

#ifndef OGRE_BUILD_COMPONENT_RTSHADERSYSTEM
void
ICSVapp::locateResources() {
  OgreBites::ApplicationContext::locateResources();
  // we have to manually specify the shaders
  Ogre::ResourceGroupManager::getSingleton().addResourceLocation(
      "../resources",
      "FileSystem",
      Ogre::ResourceGroupManager::INTERNAL_RESOURCE_GROUP_NAME);
}
#endif

void
ICSVapp::setup() {
  OgreBites::ApplicationContext::setup();
  addInputListener(this);

  // get a pointer to the already created root
  m_root   = getRoot();
  m_scnMgr = m_root->createSceneManager();

  EntityManager::Get().SetSceneManager(m_scnMgr);

  // IMGUI !!!!!!!!!!!!!!!!!!!!!!

  auto imguiOverlay = new Ogre::ImGuiOverlay();
  imguiOverlay->show();
  assert(imguiOverlay->isInitialised());
  Ogre::OverlayManager::getSingleton().addOverlay(imguiOverlay);

  addInputListener(new OgreBites::ImGuiInputListener());
  m_scnMgr->addRenderQueueListener(Ogre::OverlaySystem::getSingletonPtr());

  // IMGUI !!!!!!!!!!!!!!!!!!!!!!

  // register our scene with the RTSS
  Ogre::RTShader::ShaderGenerator* shadergen
      = Ogre::RTShader::ShaderGenerator::getSingletonPtr();
  shadergen->addSceneManager(m_scnMgr);

  Ogre::Light*     light = m_scnMgr->createLight("MainLight");
  Ogre::SceneNode* lightNode
      = m_scnMgr->getRootSceneNode()->createChildSceneNode();
  lightNode->setPosition(0, 50, 0);
  lightNode->attachObject(light);

  m_camNode = m_scnMgr->getRootSceneNode()->createChildSceneNode();
  m_camNode->setPosition(0, 0, 15);
  m_camNode->lookAt(Ogre::Vector3(0, 0, -1), Ogre::Node::TS_PARENT);

  m_cam = m_scnMgr->createCamera("myCam");
  m_cam->setNearClipDistance(0.1);
  m_cam->setFarClipDistance(10000);
  m_cam->setAutoAspectRatio(true);
  m_camNode->attachObject(m_cam);
  getRenderWindow()->addViewport(m_cam);

  IcsvEntity* ent = create_icsv_entity("ogrehead.mesh");
  ent->SetScale(0.1, 0.1, 0.1);
}

bool
ICSVapp::keyPressed(const OgreBites::KeyboardEvent& evt) {
  if (evt.keysym.sym == SDLK_KP_8)  // Camera Forward
    m_camNode->setPosition(m_camNode->getPosition() + Ogre::Vector3(0, 0, -1));

  if (evt.keysym.sym == SDLK_KP_4)  // Camera Left
    m_camNode->setPosition(m_camNode->getPosition() + Ogre::Vector3(-1, 0, 0));

  if (evt.keysym.sym == SDLK_KP_5)  // Camera Back
    m_camNode->setPosition(m_camNode->getPosition() + Ogre::Vector3(0, 0, 1));

  if (evt.keysym.sym == SDLK_KP_6)  // Camera Right
    m_camNode->setPosition(m_camNode->getPosition() + Ogre::Vector3(1, 0, 0));

  if (evt.keysym.sym == SDLK_SPACE)  // Camera Up
    m_camNode->setPosition(m_camNode->getPosition() + Ogre::Vector3(0, 1, 0));

  if (evt.keysym.sym == SDLK_LCTRL)  // Camera Down
    m_camNode->setPosition(m_camNode->getPosition() + Ogre::Vector3(0, -1, 0));

  if (evt.keysym.sym == SDLK_ESCAPE)  // Quit
    getRoot()->queueEndRendering();

  return true;
}

bool
ICSVapp::mousePressed(const OgreBites::MouseButtonEvent&) {  // FIXME
  m_LMouseDown = true;
  m_RMouseDown = true;
  return true;
}

bool
ICSVapp::mouseReleased(const OgreBites::MouseButtonEvent&) {  // FIXME
  m_LMouseDown = false;
  m_RMouseDown = false;
  return true;
}

bool
ICSVapp::mouseMoved(const OgreBites::MouseMotionEvent& evt) {  // FIXME
  if (m_LMouseDown || m_RMouseDown) {
    m_camNode->yaw(Ogre::Degree(-evt.xrel * m_rotSpd));
    m_camNode->pitch(Ogre::Degree(-evt.yrel * m_rotSpd));
  }
  return true;
}

}  // namespace ICSVapp