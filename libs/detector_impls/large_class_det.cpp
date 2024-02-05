#include "../evaluator_impls/arg_based_eval.hpp"
#include <assert.h>

#define TAG "Large Class"
#include "icsv/detector/detector.hpp"

using Structure = icsv::detector::arch::Structure;

class LargeClassDet final : public Detector {
public:
  LargeClassDet() : Detector(TAG) {}
  ~LargeClassDet() override = default;

  void DetectSmell(const ArchData& arch) override;

  void SetPublicOnly(bool p) { m_public_only = p; }

private:
  bool m_public_only = false;

private:
  auto CountStructureMethods(const Structure& strcts) -> size_t;
  auto CountStructureFields(const Structure& strcts) -> size_t;

  auto GetEvaluator(void) -> MultiArgsEvaluator* {
    auto* eval = dynamic_cast<MultiArgsEvaluator*>(Detector::GetEvaluator());
    return eval;
  }
};

static LargeClassDet* lcd = new LargeClassDet();

void
LargeClassDet::DetectSmell(const ArchData& arch) {
  auto* eval = GetEvaluator();
  assert(eval);

  for (auto& strct : arch.structures) {
    DetectorReport rep;

    std::string public_only = (m_public_only) ? "public" : "";

    auto field_count      = CountStructureFields(strct);
    auto method_count     = CountStructureMethods(strct);
    auto field_smell_lvl  = eval->EvaluateSmell("max_fields", field_count);
    auto method_smell_lvl = eval->EvaluateSmell("max_methods", method_count);

    rep.smell_tag  = TAG;
    rep.init_level = -1;
    rep.level      = std::max(field_smell_lvl, method_smell_lvl);

    rep.message = std::string("Structure `") + strct.signature + "` has "
        + std::to_string(field_count) + " " + public_only
        + " fields, with a `max_fields` Level of "
        + std::to_string(field_smell_lvl) + "\nand "
        + std::to_string(method_count) + " " + public_only
        + " methods, with a `max_methods` Level of "
        + std::to_string(method_smell_lvl) + ".";
    rep.src_info = SourceInfo(strct.src_info.file,
                              strct.src_info.line,
                              strct.src_info.col,
                              strct.signature);
    icsv::detector::register_report(TAG, rep);
  }
}

auto
LargeClassDet::CountStructureMethods(const Structure& strct) -> size_t {
  if (strct.methods.empty())
    return 0;
  if (!m_public_only)
    return strct.methods.size();

  size_t count = 0;
  for (auto& meth : strct.methods) {
    if (meth.access == "public")
      count++;
  }

  return count;
}

auto
LargeClassDet::CountStructureFields(const Structure& strct) -> size_t {
  if (strct.fields.empty())
    return 0;
  if (!m_public_only) {
    return strct.fields.size();
  }

  size_t count = 0;
  for (auto& field : strct.fields) {
    if (field.access == "public")
      count++;
  }

  return count;
}
