#include "../evaluator_impls/arg_based_eval.hpp"
#include <assert.h>

#define TAG "Long function"
#include "icsv/detector/detector.hpp"

class LongFunctionDet final : public Detector {
public:
  LongFunctionDet() : Detector(TAG) {}
  ~LongFunctionDet() override = default;

  void DetectSmell(const ArchData& arch) override;

private:
  auto GetEvaluator(void) -> MultiArgsEvaluator* {
    auto* eval = dynamic_cast<MultiArgsEvaluator*>(Detector::GetEvaluator());
    return eval;
  }
};

static LongFunctionDet* lfd = new LongFunctionDet();

void
LongFunctionDet::DetectSmell(const ArchData& arch) {
  auto* eval = GetEvaluator();
  assert(eval);

  for (auto& strct : arch.structures) {
    for (auto& meth : strct.methods) {
      DetectorReport rep;
      rep.smell_tag  = TAG;
      rep.init_level = -1;

      auto statements_lvl
          = eval->EvaluateSmell("max_statements", meth.statements);
      auto lines_lvl = eval->EvaluateSmell("max_lines", meth.lines);

      rep.level = std::max(statements_lvl, lines_lvl);

      rep.message = "Method: " + meth.signature + " has "
          + std::to_string(meth.statements)
          + " statements, with a `max_statements` Level "
          + std::to_string(statements_lvl) + ",\nand "
          + std::to_string(meth.lines) + " lines, with a `max_lines` Level "
          + std::to_string(lines_lvl) + ".";

      rep.src_info = SourceInfo(meth.src_info.file,
                                meth.src_info.line,
                                meth.src_info.col,
                                strct.signature,
                                meth.signature);

      icsv::detector::register_report(TAG, rep);
    }
  }
}