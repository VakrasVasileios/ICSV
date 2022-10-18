#include "utils/utils.hpp"
#include <assert.h>
#include <list>
#include <string>

#define TAG "Non virtual base destructor"
#include "icsv/detector/detector.hpp"

class VirtualDectructorDet : public Detector {
public:
  VirtualDectructorDet() : Detector(TAG) {}
  ~VirtualDectructorDet() override = default;

  void DetectSmell(const ArchData& arch) override;

private:
  using MethodIter    = icsv::detector::arch::Methods::const_iterator;
  using StructureIter = icsv::detector::arch::Structures::const_iterator;

private:
  MethodIter    m_meth_iter;
  StructureIter m_strct_iter;

  void SetMethIterIfNonVirtualDestructor(const std::string& base_name,
                                         const ArchData&    arch);
};

static VirtualDectructorDet* vdd = new VirtualDectructorDet();

void
VirtualDectructorDet::SetMethIterIfNonVirtualDestructor(
    const std::string& base_name,
    const ArchData&    arch) {
  m_strct_iter
      = std::find_if(arch.structures.begin(),
                     arch.structures.end(),
                     [&base_name](const auto& s) {
                       return (s.signature == base_name) ? true : false;
                     });
  assert(m_strct_iter != arch.structures.end());

  auto destructor_id = "~" + m_strct_iter->name;
  m_meth_iter        = std::find_if(m_strct_iter->methods.begin(),
                             m_strct_iter->methods.end(),
                             [&destructor_id](const auto& m) {
                               return (destructor_id == m.name) ? true : false;
                             });
}

void
VirtualDectructorDet::DetectSmell(const ArchData& arch) {
  icsv::utils::BasesList base_list = icsv::utils::get_base_structures(arch);
  for (auto& base : base_list) {
    SetMethIterIfNonVirtualDestructor(base, arch);
    if (m_meth_iter != m_strct_iter->methods.end()) {
      DetectorReport rep;
      rep.init_level = 10;
      rep.level      = 10;
      rep.message += "Destructor of base class \"" + m_strct_iter->name
          + "\" is not virtual.";
      rep.src_info = SourceInfo(m_meth_iter->src_info.file,
                                m_meth_iter->src_info.line,
                                m_meth_iter->src_info.col,
                                m_strct_iter->signature,
                                m_meth_iter->signature);
      REPORT(rep);
    }
  }
}