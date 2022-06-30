#pragma once

#include <string>

// TODO: Add interface documentation
namespace detector {

struct DetectorReport {
  std::string message;
  std::string source;
  int         level;
};

using Report = DetectorReport;

class Detector {
public:
  Detector();
  virtual ~Detector() = default;

  virtual auto SniffCode(const std::string&) -> Report = 0;
};

}  // namespace detector
