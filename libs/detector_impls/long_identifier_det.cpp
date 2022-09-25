#include "../evaluator_impls/range_based_eval.hpp"
#include <assert.h>
#include <string>

#define TAG "Excessively long identifier"
#include "icsv/detector/detector.hpp"

class LongIdentifierDet final : public Detector {
public:
  LongIdentifierDet() : Detector(TAG) {}
  ~LongIdentifierDet() override = default;

  void DetectSmell(const ArchData& arch) override;
};

static LongIdentifierDet* l_id = new LongIdentifierDet();
CREATE_RANGE_BASED_EVAL(TAG)

void
LongIdentifierDet::DetectSmell(const ArchData& arch) {
  for (auto strct : arch.structures) {

    auto           id_size = strct.name.size();
    DetectorReport strct_rep;
    strct_rep.message = "Structure \"" + strct.signature + "\" has an id of "
        + std::to_string(id_size) + " characters";
    strct_rep.init_level = id_size;
    strct_rep.level      = EVAL(id_size);
    strct_rep.src_info   = SourceInfo(strct.src_info.file,
                                    strct.src_info.line,
                                    strct.src_info.col,
                                    strct.name);
    REPORT(strct_rep);

    for (auto meth : strct.methods) {

      id_size = meth.name.size();
      DetectorReport meth_rep;
      meth_rep.message = "Method \"" + meth.signature + "\" has an id of "
          + std::to_string(id_size) + " characters";
      meth_rep.init_level = id_size;
      meth_rep.level      = id_size;
      meth_rep.src_info   = SourceInfo(strct.src_info.file,
                                     strct.src_info.line,
                                     strct.src_info.col,
                                     strct.name,
                                     meth.name);
      REPORT(meth_rep);

      for (auto arg : meth.args) {

        id_size = arg.name.size();
        DetectorReport arg_rep;
        arg_rep.message = "Argument \"" + arg.name + "\" of method \""
            + meth.signature + "\" has an id of " + std::to_string(id_size)
            + " characters";
        arg_rep.init_level = id_size;
        arg_rep.level      = EVAL(id_size);
        arg_rep.src_info   = SourceInfo(arg.src_info.file,
                                      arg.src_info.line,
                                      arg.src_info.col,
                                      strct.name,
                                      meth.name);
        REPORT(arg_rep);
      }

      for (auto def : meth.definitions) {

        id_size = def.name.size();
        DetectorReport def_rep;
        def_rep.message = "Definition \"" + def.name + "\" in method \""
            + meth.signature + "\" has an id of " + std::to_string(id_size)
            + " characters";
        def_rep.init_level = id_size;
        def_rep.level      = EVAL(id_size);
        def_rep.src_info   = SourceInfo(def.src_info.file,
                                      def.src_info.line,
                                      def.src_info.col,
                                      strct.name,
                                      meth.name);
        REPORT(def_rep);
      }
    }
    for (auto field : strct.fields) {

      id_size = field.definition.name.size();
      DetectorReport fld_rep;
      fld_rep.message = "Field \"" + field.signature + "\" of structure \""
          + strct.signature + "\" has an id of " + std::to_string(id_size)
          + " characters";
      fld_rep.init_level = id_size;
      fld_rep.level      = EVAL(id_size);
      fld_rep.src_info   = SourceInfo(field.definition.src_info.file,
                                    field.definition.src_info.line,
                                    field.definition.src_info.col,
                                    strct.name);
      REPORT(fld_rep);
    }
  }
}