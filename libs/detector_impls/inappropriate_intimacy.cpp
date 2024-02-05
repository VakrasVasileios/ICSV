#include "../evaluator_impls/range_based_eval.hpp"

#define TAG "Inappropriate Intimacy"
#include "icsv/detector/detector.hpp"

class InappropriateIntimacyDet : public Detector {
public:
  InappropriateIntimacyDet() : Detector(TAG) {}
  ~InappropriateIntimacyDet() override = default;

  void DetectSmell(const ArchData& arch) override;

private:
  auto GetEvaluator(void) -> RangeEvaluator* {
    auto* eval = dynamic_cast<RangeEvaluator*>(Detector::GetEvaluator());
    return eval;
  }
};

static InappropriateIntimacyDet* iid = new InappropriateIntimacyDet();

void
InappropriateIntimacyDet::DetectSmell(const ArchData& arch) {
  auto* eval = GetEvaluator();
  assert(eval);

  for (auto& dep : arch.dependencies) {
    auto type = std::find_if(dep.types.begin(), dep.types.end(), [](auto t1) {
      return t1.name == "MemberExpr";
    });

    if (type != dep.types.end()) {
      auto to = std::find_if(arch.structures.begin(),
                             arch.structures.end(),
                             [dep](auto s) { return s.signature == dep.to; });
      auto from
          = std::find_if(arch.structures.begin(),
                         arch.structures.end(),
                         [dep](auto s) { return s.signature == dep.from; });

      assert(to != arch.structures.end());
      assert(from != arch.structures.end());

      DetectorReport rep;
      rep.smell_tag  = TAG;
      rep.init_level = type->number;
      rep.level      = eval->EvaluateSmell(type->number);
      rep.message    = "Structure `" + dep.from + "` has "
          + std::to_string(type->number)
          + " member expressions regarding the structure `" + dep.to + "` at:\n"
          + to->src_info.file + ":" + std::to_string(to->src_info.line) + ":"
          + std::to_string(to->src_info.col) + ".";
      rep.src_info = SourceInfo(from->src_info.file,
                                from->src_info.line,
                                from->src_info.col);

      icsv::detector::register_report(TAG, rep);
    }
  }
}