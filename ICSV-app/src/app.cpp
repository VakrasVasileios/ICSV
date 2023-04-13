#include "app.hpp"
#include "icsv_entity.hpp"
#include "entity_manager.hpp"
#include <SDL2/SDL.h>

namespace ICSVapp {

bool
ICSVapp::frameStarted(const Ogre::FrameEvent& evnt) {
  OgreBites::ApplicationContext::frameStarted(evnt);

  Ogre::ImGuiOverlay::NewFrame();

  ImGui::ShowDemoWindow();
  IcsvGui::Get().Display();

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

  set_scene_manager(m_scnMgr);
  m_scnMgr->setSkyBox(true, "Examples/CloudyNoonSkyBox", 500, false);
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

  static float color[] = { 255.f, 255.f, 255.f, 255.f };
  IcsvGui::Get().SetSkyboxColorPtr(color);

  Ogre::Light*     light = m_scnMgr->createLight("MainLight");
  Ogre::SceneNode* lightNode
      = m_scnMgr->getRootSceneNode()->createChildSceneNode();
  light->setType(Ogre::Light::LT_DIRECTIONAL);
  lightNode->setDirection(1, -1, -1);
  // lightNode->setPosition(0, 0, 0);
  lightNode->attachObject(light);

  m_camNode = m_scnMgr->getRootSceneNode()->createChildSceneNode();
  m_camNode->setPosition(0, 10, 15);
  m_camNode->lookAt(Ogre::Vector3(0, 0, -1), Ogre::Node::TS_PARENT);

  m_cam = m_scnMgr->createCamera("myCam");
  m_cam->setNearClipDistance(0.1);
  m_cam->setFarClipDistance(10000);
  m_cam->setAutoAspectRatio(true);
  m_camNode->attachObject(m_cam);
  getRenderWindow()->addViewport(m_cam);

  m_camMotor = new SmoothCamMove(m_camNode);
  m_root->addFrameListener(m_camMotor);

  IcsvGui::Get().SetCameraData(m_camMotor->GetSpeedRef(),
                               m_camMotor->GetRotSmoothRef());

  // static DetectorReport rep;
  // rep.message     = "Mock report";
  // IcsvEntity* ent = create_icsv_entity(&rep);
  // ent->SetScale(1, 1, 1);
  // ent->FlipVisibility();
  m_scnMgr->showBoundingBoxes(true);
}

inline auto
operator<<(std::ostream& os, const OgreBites::KeyboardEvent& e)
    -> std::ostream& {
  return os << "\ttype: " << e.type << '\n'
            << "\tkeysym: " << (int) e.keysym.sym << std::endl;
}

bool
ICSVapp::keyReleased(const OgreBites::KeyboardEvent& evnt) {
  if (evnt.keysym.sym == 119 || evnt.keysym.sym == 115)  // Camera Forward W
    m_camMotor->HaltZ();

  if (evnt.keysym.sym == 97 || evnt.keysym.sym == 100)  // Camera Left A
    m_camMotor->HaltX();

  if (evnt.keysym.sym == SDLK_SPACE
      || evnt.keysym.sym == SDLK_LCTRL)  // Camera Up
    m_camMotor->HaltY();

  return true;
}

bool
ICSVapp::keyPressed(const OgreBites::KeyboardEvent& evnt) {
  if (evnt.keysym.sym == 119)  // Camera Forward W
    m_camMotor->Forward();

  if (evnt.keysym.sym == 97)  // Camera Left A
    m_camMotor->Left();

  if (evnt.keysym.sym == 115)  // Camera Back S
    m_camMotor->Back();

  if (evnt.keysym.sym == 100)  // Camera Right D
    m_camMotor->Right();

  if (evnt.keysym.sym == SDLK_SPACE)  // Camera Up
    m_camMotor->Up();

  if (evnt.keysym.sym == SDLK_LCTRL)  // Camera Down
    m_camMotor->Down();

  if (evnt.keysym.sym == SDLK_ESCAPE)  // Quit
    getRoot()->queueEndRendering();

  return true;
}

inline auto
operator<<(std::ostream& os, const OgreBites::MouseButtonEvent& e)
    -> std::ostream& {
  return os << "\ttype: " << e.type << '\n'
            << "\tx, y: " << e.x << ", " << e.y << '\n'
            << "\tbutton: " << (int) e.button << std::endl;
}

bool
ICSVapp::mousePressed(const OgreBites::MouseButtonEvent& evnt) {
  if (evnt.button == (unsigned char) OgreBites::ButtonType::BUTTON_LEFT) {
    m_LMouseDown = true;
    Raycast(evnt.x, evnt.y);
  }
  if (evnt.button == (unsigned char) OgreBites::ButtonType::BUTTON_RIGHT) {
    m_RMouseDown = true;
  }
  return true;
}

bool
ICSVapp::mouseReleased(const OgreBites::MouseButtonEvent& evnt) {
  if (evnt.button == (unsigned char) OgreBites::ButtonType::BUTTON_LEFT) {
    m_LMouseDown = false;
  }
  if (evnt.button == (unsigned char) OgreBites::ButtonType::BUTTON_RIGHT) {
    m_RMouseDown = false;
  }
  return true;
}

bool
ICSVapp::mouseMoved(const OgreBites::MouseMotionEvent& evnt) {
  if (m_RMouseDown) {  // FIXME: camera goes crazy after a certain point
    m_camNode->yaw(Ogre::Degree(-evnt.xrel * m_rotSpd));
    m_camNode->pitch(Ogre::Degree(-evnt.yrel * m_rotSpd));
    m_camNode->roll(Ogre::Degree(-m_camNode->getOrientation().getRoll()));
  }
  return true;
}

void
ICSVapp::Raycast(float scrn_x, float scrn_y) {
  static Ogre::Ray ray;
  ray.setOrigin(m_camNode->getPosition());
  ray.setDirection(m_cam->getCameraToViewportRay(scrn_x
                                                     / m_cam->getViewport()
                                                           ->getActualWidth(),
                                                 scrn_y
                                                     / m_cam->getViewport()
                                                           ->getActualHeight())
                   * m_cam->getFarClipDistance());
  float       dist = -1;
  const auto& entl = get_entity_list();

  for (auto* ref : entl) {
    auto res = ray.intersects(ref->GetBoundingBox());
    if (res.first) {
      if (dist < 0 || res.second > dist) {
        dist = res.second;
        IcsvGui::Get().SetReportToDisplay(ref->GetDetectorReport());
      }
    }
  }
}

bool
SmoothCamMove::frameStarted(const Ogre::FrameEvent& evt) {
  m_camNodeRef->setPosition(m_camNodeRef->getPosition()
                            + m_speed * m_dir * evt.timeSinceLastFrame);
  return true;
}

}  // namespace ICSVapp