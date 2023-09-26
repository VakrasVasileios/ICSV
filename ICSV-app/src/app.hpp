#pragma once

#include <OGRE/Bites/OgreImGuiInputListener.h>
#include <OGRE/Overlay/OgreImGuiOverlay.h>
#include <OGRE/Overlay/OgreOverlayManager.h>
#include <OGRE/Overlay/OgreOverlaySystem.h>
#include <OgreFrameListener.h>
#include <Ogre.h>
#include <OgreApplicationContext.h>
#include "gui/gui.hpp"

namespace ICSVapp {

class SmoothCamMove;

struct Mouse {
  float x_rel = 0, y_rel = 0;

  Mouse() = default;
};

class ICSVapp : public OgreBites::ApplicationContext,
                public OgreBites::InputListener {
public:
  ICSVapp() : OgreBites::ApplicationContext("ICSV") {}

  bool frameStarted(const Ogre::FrameEvent& evt);
#ifndef OGRE_BUILD_COMPONENT_RTSHADERSYSTEM
  void locateResources();
#endif
  void setup();

  bool keyPressed(const OgreBites::KeyboardEvent& evt);
  bool keyReleased(const OgreBites::KeyboardEvent& evt);
  bool mousePressed(const OgreBites::MouseButtonEvent& evt);
  bool mouseReleased(const OgreBites::MouseButtonEvent& evt);
  bool mouseMoved(const OgreBites::MouseMotionEvent& evt);

  friend class SmoothCamMove;

protected:
  struct Camera {
    Ogre::SceneNode* m_node{ nullptr };
    Ogre::Camera*    m_cam{ nullptr };
    Ogre::SceneNode* m_grid{ nullptr };
  };

private:
  Ogre::Root*         m_root{ nullptr };
  Ogre::SceneManager* m_scnMgr{ nullptr };
  Camera              m_camera;
  SmoothCamMove*      m_camMotor{ nullptr };

  bool  m_LMouseDown{ false }, m_RMouseDown{ false };
  Mouse m_mouse;

  void Raycast(float scrn_x, float scrn_y);
};

class SmoothCamMove : public Ogre::FrameListener {
public:
  SmoothCamMove(Ogre::SceneNode* camNode) : m_camNodeRef(camNode) {}
  ~SmoothCamMove() override = default;

  void Forward() { m_dir.z = -1; }
  void Back() { m_dir.z = 1; }
  void HaltZ() { m_dir.z = 0; }
  void Left() { m_dir.x = -1; }
  void Right() { m_dir.x = 1; }
  void HaltX() { m_dir.x = 0; }
  void Up() { m_dir.y = 1; }
  void Down() { m_dir.y = -1; }
  void HaltY() { m_dir.y = 0; }

  auto GetSpeedRef(void) -> float* { return &m_speed; }
  auto GetRotSpeedRef(void) -> float* { return &m_rotSpd; }

  void SetMouseRef(Mouse* m) { m_mouseRef = m; }

  void CamRotating(bool is) { m_is_rot = is; }

  bool frameStarted(const Ogre::FrameEvent& evt) override;

private:
  float            m_speed  = 2.f;
  float            m_rotSpd = 0.1f;
  bool             m_is_rot = false;
  Mouse*           m_mouseRef;
  Ogre::Vector3    m_dir{ 0, 0, 0 };
  Ogre::SceneNode* m_camNodeRef;
};

}  // namespace ICSVapp