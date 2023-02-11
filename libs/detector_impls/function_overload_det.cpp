#include "../evaluator_impls/range_based_eval.hpp"
#include "utils/utils.hpp"
#include <string>
#include <unordered_map>

#define TAG "Excessive function overloading"
#include "icsv/detector/detector.hpp"

class FuncOverloadDet : public Detector {
public:
  FuncOverloadDet() : Detector(TAG) {}
  ~FuncOverloadDet() override = default;

  void DetectSmell(const ArchData& arch) override;
};

static FuncOverloadDet* fod = new FuncOverloadDet();
CREATE_RANGE_BASED_EVAL(TAG);

void
FuncOverloadDet::DetectSmell(const ArchData& arch) {
  using OverMap = std::unordered_map<std::string, int>;

  for (auto& strct : arch.structures) {
    OverMap overloads;
    for (auto& meth : strct.methods) {
      if (icsv::utils::is_standard_class_func(meth.name, strct.name))
        continue;
      if (overloads.find(meth.name) == overloads.end())
        overloads[meth.name] = 0;
      else
        overloads[meth.name]++;
    }
    for (auto& entr : overloads) {
      DetectorReport rep;
      rep.init_level = entr.second;
      rep.level      = icsv::detector::evaluate_smell(TAG, entr.second);
      rep.message    = "Method \"" + entr.first + "\" of structure \""
          + strct.signature + "\" is overloaded " + std::to_string(entr.second)
          + " times.";
      rep.src_info = SourceInfo(strct.src_info.file,
                                strct.src_info.line,
                                strct.src_info.col,
                                strct.signature);
      icsv::detector::register_report(TAG, rep);
    }
  }
}