#include "range_based_smell_eval.hpp"

#define TAG "Too many locals"
#include "icsv/detector/detector.hpp"

class MethodLocalsDet : public Detector {
public:
  MethodLocalsDet() : Detector(TAG) {}
  ~MethodLocalsDet() override = default;

  void DetectSmell(const ArchData& arch) override;
};

static MethodLocalsDet* mlcd = new MethodLocalsDet();
CREATE_RANGE_BASED_EVAL(TAG);

void
MethodLocalsDet::DetectSmell(const ArchData& arch) {
  for (auto& strct : arch.structures) {
    for (auto& meth : strct.methods) {
      auto           defs = meth.definitions.size();
      DetectorReport rep;
      rep.init_level = defs;
      rep.level      = EVAL(defs);
      rep.message    = "Method \"" + meth.signature + "\" has "
          + std::to_string(defs) + " definitions";
      rep.src_info = SourceInfo(meth.src_info.file,
                                meth.src_info.line,
                                meth.src_info.col,
                                strct.signature,
                                meth.signature);
      REPORT(rep);
    }
  }
}