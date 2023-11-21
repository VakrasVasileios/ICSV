#include "material_factory.hpp"
#include <json/json.h>
#include <fstream>
#include <json/json.h>

namespace ICSVapp {

auto
MaterialFactory::Get() -> MaterialFactory& {
  static MaterialFactory singleton;
  return singleton;
}

auto
MaterialFactory::GetMaterialFor(const std::string& smell_name)
    -> Ogre::MaterialPtr {
  if (m_smell_materials.contains(smell_name)) {
    return Ogre::MaterialManager::getSingleton().getByName(m_smell_materials.at(
                                                               smell_name),
                                                           "ICSV_RESOURCES");
  } else
    return Ogre::MaterialManager::getSingleton().getByName("CubeMat",
                                                           "ICSV_RESOURCES");
}

void
MaterialFactory::DeseriallizeConfig(const std::string& path) {
  std::ifstream file(path);
  assert(file.is_open());
  Json::Value config_doc;
  file >> config_doc;
  assert(config_doc.isObject());
  file.close();

  assert(config_doc["smells"].isArray());

  for (auto smell : config_doc["smells"]) {

    if (smell.isMember("material")) {
      auto mat = smell["material"].asString();

      assert(
          Ogre::MaterialManager::getSingleton().getByName(mat).operator bool());

      m_smell_materials[smell["tag"].asString()] = mat;

    } else if (smell.isMember("color")) {
      auto color = smell["color"];

      auto mat_name = smell["tag"].asString();

      m_smell_materials[smell["tag"].asString()] = mat_name;
      auto mat = Ogre::MaterialManager::getSingleton().create(mat_name,
                                                              "ICSV_RESOURCES");

      assert(color.isMember("ambient"));
      auto ambient = color["ambient"];
      assert(ambient.isArray() && ambient.size() == 3);

      mat->setAmbient(ambient[0].asFloat(),
                      ambient[1].asFloat(),
                      ambient[2].asFloat());

      assert(color.isMember("diffuse"));
      auto diffuse = color["diffuse"];
      assert(diffuse.isArray() && diffuse.size() == 4);

      mat->setDiffuse(diffuse[0].asFloat(),
                      diffuse[1].asFloat(),
                      diffuse[2].asFloat(),
                      diffuse[3].asFloat());

      assert(color.isMember("specular"));
      auto specular = color["specular"];
      assert(specular.isArray() && specular.size() == 4);

      mat->setSpecular(specular[0].asFloat(),
                       specular[1].asFloat(),
                       specular[2].asFloat(),
                       specular[3].asFloat());

    } else {
      m_smell_materials[smell["tag"].asString()] = "CubeMat";
    }
  }
}

}  // namespace ICSVapp