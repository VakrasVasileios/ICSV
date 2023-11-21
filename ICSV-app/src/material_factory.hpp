#pragma once

#include <string>
#include <map>
#include <Ogre.h>

namespace ICSVapp {

class MaterialFactory final {
public:
  static auto Get() -> MaterialFactory&;

  auto GetMaterialFor(const std::string& smell_name) -> Ogre::MaterialPtr;

  void DeseriallizeConfig(const std::string& path);

private:
  std::map<std::string, std::string> m_smell_materials;

  MaterialFactory()                       = default;
  MaterialFactory(MaterialFactory&&)      = delete;
  MaterialFactory(const MaterialFactory&) = delete;
  ~MaterialFactory()                      = default;
};

}  // namespace ICSVapp