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
  m_gui.Display();

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

  m_raycaster = m_scnMgr->createRayQuery(Ogre::Ray());
  m_raycaster->setSortByDistance(true, 3);

  static DetectorReport rep;
  rep.message     = "Mock report";
  IcsvEntity* ent = create_icsv_entity("ogrehead.mesh", &rep);
  ent->SetScale(0.1, 0.1, 0.1);
}

inline auto
operator<<(std::ostream& os, const OgreBites::KeyboardEvent& e)
    -> std::ostream& {
  return os << "\ttype: " << e.type << '\n'
            << "\tkeysym: " << (int) e.keysym.sym << std::endl;
}

bool
ICSVapp::keyPressed(const OgreBites::KeyboardEvent& evnt) {
  std::cout << evnt;
  if (evnt.keysym.sym == 119)  // Camera Forward W
    m_camNode->setPosition(m_camNode->getPosition() + Ogre::Vector3(0, 0, -1));

  if (evnt.keysym.sym == 97)  // Camera Left A
    m_camNode->setPosition(m_camNode->getPosition() + Ogre::Vector3(-1, 0, 0));

  if (evnt.keysym.sym == 115)  // Camera Back S
    m_camNode->setPosition(m_camNode->getPosition() + Ogre::Vector3(0, 0, 1));

  if (evnt.keysym.sym == 100)  // Camera Right D
    m_camNode->setPosition(m_camNode->getPosition() + Ogre::Vector3(1, 0, 0));

  if (evnt.keysym.sym == SDLK_SPACE)  // Camera Up
    m_camNode->setPosition(m_camNode->getPosition() + Ogre::Vector3(0, 1, 0));

  if (evnt.keysym.sym == SDLK_LCTRL)  // Camera Down
    m_camNode->setPosition(m_camNode->getPosition() + Ogre::Vector3(0, -1, 0));

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
ICSVapp::mousePressed(const OgreBites::MouseButtonEvent& evnt) {  // FIXME
  if (evnt.button == (unsigned char) OgreBites::ButtonType::BUTTON_LEFT) {
    m_LMouseDown = true;
    BeginRayCastProcessAt(evnt.x, evnt.y);
  }
  if (evnt.button == (unsigned char) OgreBites::ButtonType::BUTTON_RIGHT) {
    m_RMouseDown = true;
  }
  return true;
}

bool
ICSVapp::mouseReleased(const OgreBites::MouseButtonEvent& evnt) {  // FIXME
  if (evnt.button == (unsigned char) OgreBites::ButtonType::BUTTON_LEFT) {
    m_LMouseDown = false;
  }
  if (evnt.button == (unsigned char) OgreBites::ButtonType::BUTTON_RIGHT) {
    m_RMouseDown = false;
  }
  return true;
}

bool
ICSVapp::mouseMoved(const OgreBites::MouseMotionEvent& evnt) {  // FIXME
  if (m_RMouseDown) {
    m_camNode->yaw(Ogre::Degree(-evnt.xrel * m_rotSpd));
    m_camNode->pitch(Ogre::Degree(-evnt.yrel * m_rotSpd));
    m_camNode->roll(-m_camNode->getOrientation().getRoll());
  }
  return true;
}

void
ICSVapp::BeginRayCastProcessAt(int xPix, int yPix) {
  static Ogre::Ray ray;

  // screen to world coordinates
  float frayX = (m_cam->getAspectRatio() * xPix + 0.5f)
          / (m_cam->getViewport()->getActualWidth() / 2)
      - 1.0f;
  float frayY = 1.0f
      - (m_cam->getAspectRatio() * yPix + 0.5f)
          / (m_cam->getViewport()->getActualHeight() / 2);

  auto rayDir
      = Ogre::Vector3(frayX - m_camNode->getPosition().x,
                      frayY - m_camNode->getPosition().y,
                      m_cam->getFarClipDistance() - m_camNode->getPosition().z)
      * m_cam->getDerivedDirection();
  rayDir.normalise();
  ray.setDirection(rayDir);
  m_raycaster->setRay(ray);
  auto results = m_raycaster->execute();  // get sorted by distance results
  if (!results.empty()) {
    auto entt_pos = results.front().movable->getParentNode()->getPosition();
    static auto entt_pos_pred = [&entt_pos](auto ref) -> bool {
      if (ref->GetPosition() == entt_pos)
        return true;
      else
        return false;
    };
    auto entt = find_entt_if(entt_pos_pred);
    if (entt != nullptr) {  // check if object hit is IcsvEntity
                            // pass report to gui display
      m_gui.SetReportToDisplay(entt->GetDetectorReport());
    }
  }
}

}  // namespace ICSVapp