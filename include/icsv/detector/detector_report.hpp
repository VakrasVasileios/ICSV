#pragma once

#include <string>

namespace icsv::detector {

typedef struct DetectorReport {
  std::string message = "";
  std::string source  = "";
  int         level   = -1;
} Report;

}  // namespace icsv::detector