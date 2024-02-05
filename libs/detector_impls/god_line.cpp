#include "../evaluator_impls/range_based_eval.hpp"
#include <fstream>

#define TAG "Excessively long line of code"
#include "icsv/detector/detector.hpp"

class GodLineDet final : public Detector {
public:
  GodLineDet() : Detector(TAG) {}
  ~GodLineDet() override = default;

  void DetectSmell(const ArchData& arch) override;

private:
  auto GetEvaluator(void) -> RangeEvaluator* {
    auto* eval = dynamic_cast<RangeEvaluator*>(Detector::GetEvaluator());
    return eval;
  }

  void IterateFile(const std::string& path);
};

static GodLineDet* gl = new GodLineDet();

void
GodLineDet::IterateFile(const std::string& path) {
  static auto* eval = GetEvaluator();
  assert(eval);

  std::ifstream f(path);
  assert(f.is_open());

  std::string line;

  size_t line_c         = 0;
  size_t multil_comment = 0;

  while (std::getline(f, line)) {
    line_c++;
    if (line.starts_with("/*")) {
      multil_comment++;
    }
    if (line.find("*/") != line.npos) {
      multil_comment--;
    }
    if (line.starts_with("//") == false && multil_comment == 0) {
      DetectorReport rep;

      rep.smell_tag  = TAG;
      rep.init_level = line.size();
      rep.level      = eval->EvaluateSmell(rep.init_level);
      rep.message    = "Line " + std::to_string(line_c) + " of " + path
          + "\nhas an un-commented length of " + std::to_string(line.size())
          + " characters.";
      rep.src_info = SourceInfo(path, line_c);

      icsv::detector::register_report(TAG, rep);
    }

    line.clear();
  }

  f.close();
}

void
GodLineDet::DetectSmell(const ArchData& arch) {
  for (auto& source : arch.sources) {
    IterateFile(source);
  }
  for (auto& header : arch.headers) {
    IterateFile(header);
  }
}