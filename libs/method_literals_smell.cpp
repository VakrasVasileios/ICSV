#include "range_based_smell_eval.hpp"

#define TAG "Too many function literals"
#include "icsv/detector/detector.hpp"

class MethodLiteralsDet : public Detector {
public:
  MethodLiteralsDet() : Detector(TAG) {}
  ~MethodLiteralsDet() override = default;

  void DetectSmell(const ArchData& arch) override;
};

static MethodLiteralsDet* mltd = new MethodLiteralsDet();
CREATE_RANGE_BASED_EVAL(TAG);

void
MethodLiteralsDet::DetectSmell(const ArchData& arch) {
  for (auto& strct : arch.structures) {
    for (auto& meth : strct.methods) {
      auto           literals = meth.literals;
      DetectorReport rep;
      rep.init_level = literals;
      rep.level      = EVAL(literals);
      rep.message    = "Method \"" + meth.signature + "\" has "
          + std::to_string(literals) + " literals";
      rep.src_info = SourceInfo(meth.src_info.file,
                                meth.src_info.line,
                                meth.src_info.col,
                                strct.signature,
                                meth.signature);
      REPORT(rep);
    }
  }
}
