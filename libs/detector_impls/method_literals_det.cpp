#include "../evaluator_impls/range_based_eval.hpp"

#define TAG "Too many function literals"
#include "icsv/detector/detector.hpp"

class MethodLiteralsDet : public Detector {
public:
  MethodLiteralsDet() : Detector(TAG) {}
  ~MethodLiteralsDet() override = default;

  void DetectSmell(const ArchData& arch) override;
};

static MethodLiteralsDet* mltd = new MethodLiteralsDet();

void
MethodLiteralsDet::DetectSmell(const ArchData& arch) {
  for (auto& strct : arch.structures) {
    for (auto& meth : strct.methods) {
      auto           literals = meth.literals;
      DetectorReport rep;
      rep.init_level = literals;
      rep.level      = icsv::detector::evaluate_smell(TAG, literals);
      rep.message    = "Method `" + meth.signature + "` has "
          + std::to_string(literals) + " literals";
      rep.src_info = SourceInfo(meth.src_info.file,
                                meth.src_info.line,
                                meth.src_info.col,
                                strct.signature,
                                meth.signature);
      icsv::detector::register_report(TAG, rep);
    }
  }
}
