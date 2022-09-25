#include "../evaluator_impls/range_based_eval.hpp"

#define TAG "Too many parameters"
#include "icsv/detector/detector.hpp"

class MethodArgsDet : public Detector {
public:
  MethodArgsDet() : Detector(TAG) {}
  ~MethodArgsDet() override = default;

  void DetectSmell(const ArchData& arch) override;
};

static MethodArgsDet* mad = new MethodArgsDet();
CREATE_RANGE_BASED_EVAL(TAG);

void
MethodArgsDet::DetectSmell(const ArchData& arch) {
  for (auto& strct : arch.structures) {
    for (auto& meth : strct.methods) {
      auto argc = meth.args.size();
      if (argc == 0)
        continue;
      DetectorReport rep;
      rep.init_level = argc;
      rep.level      = EVAL(argc);
      rep.message    = "Method \"" + meth.signature + "\" has "
          + std::to_string(argc) + " arguments";
      rep.src_info = SourceInfo(meth.src_info.file,
                                meth.src_info.line,
                                meth.src_info.col,
                                strct.signature,
                                meth.signature);
      REPORT(rep);
    }
  }
}