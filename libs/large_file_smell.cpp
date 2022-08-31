#include "icsv/detector/detector.hpp"
#include <fstream>
#include <string>

unsigned
file_line_count(const std::string& path) {
  unsigned      fcount = 0;
  std::ifstream file(path);
  std::string   dump;
  while (std::getline(file, dump))
    ++fcount;

  return fcount;
}

class LargeFileDetector : public icsv::detector::Detector {
public:
  LargeFileDetector() : icsv::detector::Detector("large_file_det") {}
  ~LargeFileDetector() override = default;

  void SmellDetect(const Json::Value& arch);
};

LargeFileDetector lgd;

void
LargeFileDetector::SmellDetect(const Json::Value& arch) {
  for (auto& src : arch["sources"]) {
    DetectorReport rep;
    unsigned       lines
        = file_line_count(src.asString());  // FIXME: This will need the full,
                                            // json has only file name
    rep.level   = lines;
    rep.message = "Source file: " + src.asString() + " has "
        + std::to_string(lines) + " of code.";
    rep.source = src.asString();
    REGISTER_REPORT("large_file", rep);
  }
};
