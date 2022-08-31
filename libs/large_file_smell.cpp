#include "icsv/detector/detector.hpp"
#include <assert.h>
#include <fstream>
#include <string>

class LargeFileDet : public icsv::detector::Detector {
public:
  LargeFileDet() : icsv::detector::Detector("large_file_det") {}
  ~LargeFileDet() override = default;

  void SmellDetect(const Json::Value& arch);

private:
  unsigned FileLineCount(const std::string& path);
};

LargeFileDet* lgd = new LargeFileDet();

unsigned
LargeFileDet::FileLineCount(const std::string& path) {
  unsigned      fcount = 0;
  std::ifstream file(path);
  assert(file.is_open());
  std::string dump;
  while (std::getline(file, dump))
    ++fcount;

  return fcount;
}

void
LargeFileDet::SmellDetect(const Json::Value& arch) {
  for (auto& src : arch["sources"]) {
    DetectorReport rep;
    unsigned       lines
        = FileLineCount(src.asString());  // FIXME: This will need the full,
                                          // json has only file name
    rep.level   = lines;
    rep.message = "Source file: " + src.asString() + " has "
        + std::to_string(lines) + " of code.";
    rep.src_info.file = src.asString();
    REGISTER_REPORT("large_file", rep);
  }
};
