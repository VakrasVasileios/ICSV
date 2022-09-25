#include "../evaluator_impls/range_based_eval.hpp"
#include <string>
#include <unordered_map>

#define TAG "Excessive function overloading"
#include "icsv/detector/detector.hpp"

class FuncOverloadDet : public Detector {
public:
  FuncOverloadDet() : Detector(TAG) {}
  ~FuncOverloadDet() override = default;

  void DetectSmell(const ArchData& arch) override;

private:
  bool IsClassFunc(const std::string& meth_name, const std::string& strct_name);
};

static FuncOverloadDet* fod = new FuncOverloadDet();
CREATE_RANGE_BASED_EVAL(TAG);

bool
FuncOverloadDet::IsClassFunc(const std::string& meth_name,
                             const std::string& strct_name) {
  if (meth_name == strct_name)
    return true;
  if (meth_name == ("~" + strct_name))
    return true;
  if (meth_name.starts_with("operator"))
    return true;
  return false;
}

void
FuncOverloadDet::DetectSmell(const ArchData& arch) {
  using OverMap = std::unordered_map<std::string, int>;

  for (auto& strct : arch.structures) {
    OverMap overloads;
    for (auto& meth : strct.methods) {
      if (IsClassFunc(meth.name, strct.name))
        continue;
      if (overloads.find(meth.name) == overloads.end())
        overloads[meth.name] = 0;
      else
        overloads[meth.name]++;
    }
    for (auto& entr : overloads) {
      DetectorReport rep;
      rep.init_level = entr.second;
      rep.level      = EVAL(entr.second);
      rep.message    = "Method \"" + entr.first + "\" of structure \""
          + strct.signature + "\" is overloaded " + std::to_string(entr.second)
          + " times.";
      rep.src_info = SourceInfo(strct.src_info.file,
                                strct.src_info.line,
                                strct.src_info.col,
                                strct.signature);
      REPORT(rep);
    }
  }
}