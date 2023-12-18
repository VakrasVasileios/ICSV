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

      auto mat_name = smell["tag"].asString();

      auto omat
          = Ogre::MaterialManager::getSingleton().create(mat_name,
                                                         "ICSV_RESOURCES");

      MaterialFacPtr mat = std::make_shared<Material>(omat);

      m_smell_materials[smell["tag"].asString()] = mat;

      assert(color.isMember("ambient"));
      auto ambient = color["ambient"];
      assert(ambient.isArray() && ambient.size() == 3);

      mat->m_material->setAmbient(ambient[0].asFloat(),
                                  ambient[1].asFloat(),
                                  ambient[2].asFloat());

      mat->m_color[AMBIENT][0] = ambient[0].asFloat();
      mat->m_color[AMBIENT][1] = ambient[1].asFloat();
      mat->m_color[AMBIENT][2] = ambient[2].asFloat();
      mat->m_color[AMBIENT][3] = 1.f;

      if (color.isMember("diffuse")) {
        auto diffuse = color["diffuse"];
        assert(diffuse.isArray() && diffuse.size() == 4);

        mat->m_material->setDiffuse(diffuse[0].asFloat(),
                                    diffuse[1].asFloat(),
                                    diffuse[2].asFloat(),
                                    diffuse[3].asFloat());

        mat->m_color[DIFFUSE][0] = diffuse[0].asFloat();
        mat->m_color[DIFFUSE][1] = diffuse[1].asFloat();
        mat->m_color[DIFFUSE][2] = diffuse[2].asFloat();
        mat->m_color[DIFFUSE][3] = diffuse[3].asFloat();
      }

      if (color.isMember("specular")) {
        auto specular = color["specular"];
        assert(specular.isArray() && specular.size() == 4);

        mat->m_material->setSpecular(specular[0].asFloat(),
                                     specular[1].asFloat(),
                                     specular[2].asFloat(),
                                     specular[3].asFloat());

        mat->m_color[SPECULAR][0] = specular[0].asFloat();
        mat->m_color[SPECULAR][1] = specular[1].asFloat();
        mat->m_color[SPECULAR][2] = specular[2].asFloat();
        mat->m_color[SPECULAR][3] = specular[3].asFloat();
      }

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