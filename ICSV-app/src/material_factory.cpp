#include "material_factory.hpp"
#include <memory>
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
    -> MaterialFacPtr {
  assert(m_smell_materials.contains(smell_name));
  return m_smell_materials.at(smell_name);
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
      auto mat_tag = smell["material"].asString();

      auto mat = Ogre::MaterialManager::getSingleton().getByName(mat_tag);
      assert(mat.operator bool());

      m_smell_materials[smell["tag"].asString()]
          = std::make_shared<Material>(mat);

    } else if (smell.isMember("color")) {
      auto color = smell["color"];
      assert(color.isArray() && color.size() == 3);

      auto mat_name = smell["tag"].asString();

      auto omat
          = Ogre::MaterialManager::getSingleton().create(mat_name,
                                                         "ICSV_RESOURCES");

      MaterialFacPtr mat = std::make_shared<Material>(omat);

      m_smell_materials[smell["tag"].asString()] = mat;

      mat->m_material->setAmbient(color[0].asFloat(),
                                  color[1].asFloat(),
                                  color[2].asFloat());
      mat->m_material->setDiffuse(color[0].asFloat(),
                                  color[1].asFloat(),
                                  color[2].asFloat(),
                                  1.0f);
      mat->m_material->setSpecular(color[0].asFloat(),
                                   color[1].asFloat(),
                                   color[2].asFloat(),
                                   1.0f);

      mat->m_color[0] = color[0].asFloat();
      mat->m_color[1] = color[1].asFloat();
      mat->m_color[2] = color[2].asFloat();

    } else {
      auto mat_name = std::string(smell["tag"].asString());
      auto omat
          = Ogre::MaterialManager::getSingleton().create(mat_name,
                                                         "ICSV_RESOURCES");
      m_smell_materials[smell["tag"].asString()]
          = std::make_shared<Material>(omat);
    }
  }
}

}  // namespace ICSVapp