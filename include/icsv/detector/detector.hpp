#pragma once

#include <string>

namespace detector {

class Detector {
public:
  Detector()          = default;
  virtual ~Detector() = default;

  virtual int SniffCode(const std::string&) = 0;
};

}  // namespace detector
