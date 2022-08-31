#pragma once

#include <iostream>
#include <string>

namespace icsv::detector {

typedef struct DetectorReport {
  std::string message = "";
  std::string source  = "";
  int         level   = -1;
} Report;

inline auto
operator<<(std::ostream& os, const icsv::detector::Report& rep)
    -> std::ostream& {
  return os << "Message: " << rep.message << '\n'
            << "Source: " << rep.source << '\n'
            << "Level: " << rep.level << '\n';
}

}  // namespace icsv::detector