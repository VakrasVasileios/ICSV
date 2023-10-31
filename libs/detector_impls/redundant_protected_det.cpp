#include "../evaluator_impls/range_based_eval.hpp"
#include "utils/utils.hpp"
#include <assert.h>

#define TAG "Redundant protected access"
#include "icsv/detector/detector.hpp"

class RedundantProtectedDet : public Detector {
public:
  RedundantProtectedDet() : Detector(TAG) {}
  ~RedundantProtectedDet() override = default;

  void DetectSmell(const ArchData& arch) override;
};

static RedundantProtectedDet* rpd = new RedundantProtectedDet();

void
RedundantProtectedDet::DetectSmell(const ArchData& arch) {
  icsv::utils::BasesList blist = icsv::utils::get_base_structures(arch);

  for (auto strct : arch.structures) {
    if (std::find_if(blist.begin(),
                     blist.end(),
                     [&strct](const auto& b) {
                       return strct.signature == b ? true : false;
                     })
        == blist.end())
      continue;

    int protected_members = 0;
    for (auto meth : strct.methods) {
      if (meth.access == "protected")
        protected_members++;
    }
    for (auto field : strct.fields) {
      if (field.access == "protected")
        protected_members++;
    }

    DetectorReport rep;
    rep.init_level = protected_members;
    rep.level      = icsv::detector::evaluate_smell(TAG, protected_members);
    rep.message    = "Class \"" + strct.signature
        + "\" is not inherited from and has "
        + std::to_string(protected_members)
        + " protected variables or methods.";
    rep.src_info = SourceInfo(strct.src_info.file,
                              strct.src_info.line,
                              strct.src_info.col,
                              strct.signature);
    icsv::detector::register_report(TAG, rep);
  }
}
