#pragma once

#include <string>
#include <map>
#include <Ogre.h>

namespace ICSVapp {

enum ColorType : int { AMBIENT = 0, DIFFUSE, SPECULAR };

struct Material {
  Material(Ogre::MaterialPtr m) : m_material(m) {}
  Ogre::MaterialPtr m_material;
  float             m_color[3][4];
};

using MaterialFacPtr = std::shared_ptr<Material>;

class MaterialFactory final {
public:
public:
  static auto Get() -> MaterialFactory&;

  auto GetMaterialFor(const std::string& smell_name) -> MaterialFacPtr;

  void DeseriallizeConfig(const std::string& path);

private:
  std::map<std::string, MaterialFacPtr> m_smell_materials;

  MaterialFactory()                       = default;
  MaterialFactory(MaterialFactory&&)      = delete;
  MaterialFactory(const MaterialFactory&) = delete;
  ~MaterialFactory()                      = default;
};

}  // namespace ICSVapp