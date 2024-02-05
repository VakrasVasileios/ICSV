#include "../evaluator_impls/arg_based_eval.hpp"

#define TAG "Cyclomatic complexity"
#include "icsv/detector/detector.hpp"

class CycloComplexDet : public Detector {
public:
  CycloComplexDet() : Detector(TAG) {}
  ~CycloComplexDet() override = default;

  void DetectSmell(const ArchData& arch) override;

private:
  auto GetEvaluator(void) -> MultiArgsEvaluator* {
    auto* eval = dynamic_cast<MultiArgsEvaluator*>(Detector::GetEvaluator());
    return eval;
  }
};

static CycloComplexDet* ccd = new CycloComplexDet();

void
CycloComplexDet::DetectSmell(const ArchData& arch) {
  auto* eval = GetEvaluator();
  assert(eval);

  for (auto& strct : arch.structures) {
    for (auto& meth : strct.methods) {
      DetectorReport rep;
      rep.smell_tag  = TAG;
      rep.init_level = -1;

      auto loop_lvl   = eval->EvaluateSmell("max_loops", meth.loops);
      auto branch_lvl = eval->EvaluateSmell("max_branches", meth.branches);

      rep.level   = std::max(loop_lvl, branch_lvl);
      rep.message = "Method `" + meth.signature + "` has "
          + std::to_string(meth.loops)
          + " loop statements, with a `max_loops` Level of "
          + std::to_string(loop_lvl) + ",\nand " + std::to_string(meth.branches)
          + " branching statements, with a `max_branches` Level of "
          + std::to_string(branch_lvl) + ".";
      rep.src_info = SourceInfo(meth.src_info.file,
                                meth.src_info.line,
                                meth.src_info.col,
                                strct.signature,
                                meth.signature);

      icsv::detector::register_report(TAG, rep);
    }
  }
}