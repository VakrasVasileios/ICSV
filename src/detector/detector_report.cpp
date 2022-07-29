#include "icsv/detector/detector_report.hpp"
#include <iostream>

namespace icsv::detector {

auto
operator<<(std::ostream& os, const Report& rep) -> std::ostream& {
  os << "Message: " << rep.message << '\n';
  os << "Source: " << rep.source << '\n';
  os << "Level: " << rep.level << '\n';
  return os;
}

}  // namespace icsv::detector
