#pragma once

#include <OGRE/Bites/OgreImGuiInputListener.h>
#include <OGRE/Overlay/OgreImGuiOverlay.h>
#include <OGRE/Overlay/OgreOverlayManager.h>
#include <OGRE/Overlay/OgreOverlaySystem.h>
#include <Ogre.h>
#include <OgreApplicationContext.h>
#include "gui/gui.hpp"

namespace ICSVapp {

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
  bool mousePressed(const OgreBites::MouseButtonEvent& evt);
  bool mouseReleased(const OgreBites::MouseButtonEvent& evt);
  bool mouseMoved(const OgreBites::MouseMotionEvent& evt);

private:
  Ogre::Root*         m_root{ nullptr };
  Ogre::SceneManager* m_scnMgr{ nullptr };
  Ogre::SceneNode*    m_camNode{ nullptr };
  Ogre::Camera*       m_cam{ nullptr };
  IcsvGui m_gui;

  float m_rotSpd = 0.1f;
  bool  m_LMouseDown{ false }, m_RMouseDown{ false };
};

}  // namespace ICSVapp