#include "../evaluator_impls/range_based_eval.hpp"

#define TAG "Too many locals"
#include "icsv/detector/detector.hpp"

class MethodLocalsDet : public Detector {
public:
  MethodLocalsDet() : Detector(TAG) {}
  ~MethodLocalsDet() override = default;

  void DetectSmell(const ArchData& arch) override;
};

static MethodLocalsDet* mlcd = new MethodLocalsDet();

void
MethodLocalsDet::DetectSmell(const ArchData& arch) {
  for (auto& strct : arch.structures) {
    for (auto& meth : strct.methods) {
      auto           defs = meth.definitions.size();
      DetectorReport rep;
      rep.init_level = defs;
      rep.level      = icsv::detector::evaluate_smell(TAG, defs);
      rep.message    = "Method \"" + meth.signature + "\" has "
          + std::to_string(defs) + " definitions";
      rep.src_info = SourceInfo(meth.src_info.file,
                                meth.src_info.line,
                                meth.src_info.col,
                                strct.signature,
                                meth.signature);

      icsv::detector::register_report(TAG, rep);
    }
  }
}