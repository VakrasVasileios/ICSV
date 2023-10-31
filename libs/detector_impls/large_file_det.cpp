#include "../evaluator_impls/range_based_eval.hpp"
#include <assert.h>
#include <fstream>
#include <string>

#define TAG "Large File"
#include "icsv/detector/detector.hpp"

class LargeFileDet final : public Detector {
public:
  LargeFileDet() : Detector(TAG) {}
  ~LargeFileDet() override = default;

  void DetectSmell(const ArchData& arch) override;

private:
  unsigned FileLineCount(const std::string& path);
};

static LargeFileDet* lgd = new LargeFileDet();

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
    unsigned lines = FileLineCount(src);  // FIXME: This will need the full
                                          // path, json has only file name
    rep.init_level = lines;
    rep.level      = icsv::detector::evaluate_smell(TAG, lines);
    rep.message
        = "Source file: " + src + " has " + std::to_string(lines) + " of code.";
    rep.src_info.file = src;

    icsv::detector::register_report(TAG, rep);
  }
};
