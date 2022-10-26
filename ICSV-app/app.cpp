#include "gui/gui.hpp"
#include <OGRE/Bites/OgreImGuiInputListener.h>
#include <OGRE/Overlay/OgreImGuiOverlay.h>
#include <OGRE/Overlay/OgreOverlayManager.h>
#include <OGRE/Overlay/OgreOverlaySystem.h>
#include <Ogre.h>
#include <OgreApplicationContext.h>

class ICSVapp : public OgreBites::ApplicationContext,
                public OgreBites::InputListener {
public:
  ICSVapp() : OgreBites::ApplicationContext("OgreImguiExample") {}

  bool frameStarted(const Ogre::FrameEvent& evt) {
    OgreBites::ApplicationContext::frameStarted(evt);

    Ogre::ImGuiOverlay::NewFrame();

    ImGui::ShowDemoWindow();
    show_menu();

    return true;
  }

#ifndef OGRE_BUILD_COMPONENT_RTSHADERSYSTEM
  void locateResources() {
    OgreBites::ApplicationContext::locateResources();
    // we have to manually specify the shaders
    Ogre::ResourceGroupManager::getSingleton().addResourceLocation(
        "../resources",
        "FileSystem",
        Ogre::ResourceGroupManager::INTERNAL_RESOURCE_GROUP_NAME);
  }
#endif

  void setup() {
    OgreBites::ApplicationContext::setup();
    addInputListener(this);

    // get a pointer to the already created root
    Ogre::Root*         root   = getRoot();
    Ogre::SceneManager* scnMgr = root->createSceneManager();

    // IMGUI !!!!!!!!!!!!!!!!!!!!!!

    auto imguiOverlay = new Ogre::ImGuiOverlay();
    imguiOverlay->show();
    assert(imguiOverlay->isInitialised());
    Ogre::OverlayManager::getSingleton().addOverlay(imguiOverlay);

    addInputListener(new OgreBites::ImGuiInputListener());
    scnMgr->addRenderQueueListener(Ogre::OverlaySystem::getSingletonPtr());

    // IMGUI !!!!!!!!!!!!!!!!!!!!!!

    // register our scene with the RTSS
    Ogre::RTShader::ShaderGenerator* shadergen
        = Ogre::RTShader::ShaderGenerator::getSingletonPtr();
    shadergen->addSceneManager(scnMgr);

    Ogre::Light*     light = scnMgr->createLight("MainLight");
    Ogre::SceneNode* lightNode
        = scnMgr->getRootSceneNode()->createChildSceneNode();
    lightNode->setPosition(0, 10, 15);
    lightNode->attachObject(light);

    Ogre::SceneNode* camNode
        = scnMgr->getRootSceneNode()->createChildSceneNode();
    camNode->setPosition(0, 0, 15);
    camNode->lookAt(Ogre::Vector3(0, 0, -1), Ogre::Node::TS_PARENT);

    Ogre::Camera* cam = scnMgr->createCamera("myCam");
    cam->setNearClipDistance(5);  // specific to this sample
    cam->setAutoAspectRatio(true);
    camNode->attachObject(cam);
    getRenderWindow()->addViewport(cam);

    Ogre::Entity*    ent  = scnMgr->createEntity("Sinbad.mesh");
    Ogre::SceneNode* node = scnMgr->getRootSceneNode()->createChildSceneNode();
    node->attachObject(ent);
  }

  bool keyPressed(const OgreBites::KeyboardEvent& evt) {
    if (evt.keysym.sym == 27) {
      getRoot()->queueEndRendering();
    }

    return true;
  }
};

int
main(void) {
  ICSVapp app;
  app.initApp();
  app.getRoot()->startRendering();
  app.closeApp();
  return 0;
}
