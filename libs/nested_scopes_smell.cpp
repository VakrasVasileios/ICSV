#include "range_based_smell_eval.hpp"
#include <assert.h>

#define TAG "Nested Scopes"
#include "icsv/detector/detector.hpp"

class NestedScopesDet : Detector {
public:
  NestedScopesDet() : Detector(TAG) {}
  ~NestedScopesDet() override = default;

  void DetectSmell(const ArchData& arch) override;
};

static NestedScopesDet* nsd = new NestedScopesDet();
CREATE_RANGE_BASED_EVAL(TAG);

void
NestedScopesDet::DetectSmell(const ArchData& arch) {
  for (auto& strct : arch.structures) {
    for (auto& meth : strct.methods) {
      DetectorReport rep;
      rep.init_level = meth.max_scope;
      rep.level      = EVAL(meth.max_scope);
      rep.message    = "Method \"" + meth.signature + "\" has "
          + std::to_string(rep.level) + " nested scopes.";
      rep.src_info = SourceInfo(meth.src_info.file,
                                meth.src_info.line,
                                meth.src_info.col,
                                strct.signature,
                                meth.signature);
      REPORT(rep);
    }
  }
}