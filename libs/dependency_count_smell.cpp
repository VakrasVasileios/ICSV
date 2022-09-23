#include "range_based_smell_eval.hpp"
#include <string>
#include <unordered_map>
#include <vector>

#define TAG "Many Dependencies"
#include "icsv/detector/detector.hpp"

class DependencyCountDet : Detector {
public:
  DependencyCountDet() : Detector(TAG) {}
  ~DependencyCountDet() override = default;

  void DetectSmell(const ArchData& arch) override;
};

static DependencyCountDet* dcd = new DependencyCountDet();
CREATE_RANGE_BASED_EVAL(TAG);

void
DependencyCountDet::DetectSmell(const ArchData& arch) {
  using DepMap = std::unordered_map<std::string, std::vector<std::string>>;
  DepMap dep_map;

  for (auto& dep : arch.dependencies)
    dep_map[dep.from].emplace_back(dep.to);

  for (auto& dep_str : dep_map) {
    DetectorReport rep;

    rep.init_level        = dep_str.second.size();
    rep.level             = EVAL(dep_str.second.size());
    std::string dep_names = "";
    for (std::size_t i = 0; i < dep_str.second.size() - 1; i++)
      dep_names += dep_str.second[i] + ", ";
    dep_names += dep_str.second[dep_str.second.size() - 1];

    auto strct
        = std::find_if(arch.structures.begin(),
                       arch.structures.end(),
                       [dep_str](const auto& m) {
                         return (m.signature == dep_str.first) ? true : false;
                       });
    rep.message = "Structure \"" + dep_str.first + "\" directly depends upon "
        + std::to_string(dep_str.second.size())
        + " other classes. These are: " + dep_names;
    rep.src_info = SourceInfo(strct->src_info.file,
                              strct->src_info.line,
                              strct->src_info.col,
                              dep_str.first);
    REPORT(rep);
  }
}