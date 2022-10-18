#include "utils/utils.hpp"

namespace icsv::utils {

auto
get_base_structures(const ArchData& arch) -> BasesList {
  BasesList base_list;
  for (auto& dep : arch.dependencies) {
    if (std::find_if(dep.types.begin(),
                     dep.types.end(),
                     [](const auto& t) {
                       return t.name == "Inherit" ? true : false;
                     })
        != dep.types.end())
      base_list.emplace_back(dep.to);
  }
  return base_list;
}

bool
is_standard_class_func(const std::string& meth_name,
                       const std::string& strct_name) {
  if (meth_name == strct_name)
    return true;
  if (meth_name == ("~" + strct_name))
    return true;
  if (meth_name.starts_with("operator"))
    return true;
  return false;
}

}  // namespace icsv::utils