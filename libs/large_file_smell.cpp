#include "icsv/detector/detector.hpp"
#include <assert.h>
#include <fstream>
#include <string>

#define TAG "Large File"

class LargeFileDet final : public icsv::detector::Detector {
public:
  LargeFileDet() : icsv::detector::Detector(TAG) {}
  ~LargeFileDet() override = default;

  void DetectSmell(const ArchData& arch);

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
LargeFileDet::DetectSmell(const ArchData& arch) {
  for (auto& src : arch.sources) {
    DetectorReport rep;
    unsigned lines = FileLineCount(src);  // FIXME: This will need the full,
                                          // json has only file name
    rep.level = lines;
    rep.message
        = "Source file: " + src + " has " + std::to_string(lines) + " of code.";
    rep.src_info.file = src;
    REGISTER_REPORT(TAG, rep);
  }
};
