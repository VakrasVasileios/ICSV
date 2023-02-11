#pragma once

#include <OgreSceneQuery.h>

namespace ICSVapp {

class RayCaster : public Ogre::RaySceneQuery {
public:
  RayCaster(Ogre::SceneManager* creator) : Ogre::RaySceneQuery(creator) {}
  ~RayCaster() override = default;

  void execute(Ogre::RaySceneQueryListener*) override {}
};

#if (0)
class RayCasterListener : public Ogre::RaySceneQueryListener {  // TODO
public:
  RayCasterListener() : Ogre::RaySceneQueryListener() {}
  ~RayCasterListener() override = default;

  bool queryResult(Ogre::MovableObject* obj, Ogre::Real distance) override;
  bool queryResult(Ogre::SceneQuery::WorldFragment* fragment,
                   Ogre::Real                       distance) override {
    return false;
  }
};
#endif

}  // namespace ICSVapp
