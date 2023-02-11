#include "../evaluator_impls/regex_based_eval.hpp"
#include "utils/utils.hpp"

#define TAG "Naming conventions"
#include "icsv/detector/detector.hpp"

class NamingConventionsDet : public Detector {
public:
  NamingConventionsDet() : Detector(TAG) {}
  ~NamingConventionsDet() override = default;

  void DetectSmell(const ArchData& arch) override;
};

static NamingConventionsDet* ncd = new NamingConventionsDet();
CREATE_REGEX_BASED_EVAL(TAG);

void
NamingConventionsDet::DetectSmell(const ArchData& arch) {
  for (auto& strct : arch.structures) {
    DetectorReport strct_rep;
    strct_rep.init_level = eval->EvaluateClassName(strct.name);
    strct_rep.level = icsv::detector::evaluate_smell(TAG, strct_rep.init_level);
    strct_rep.message = "Structure \"" + strct.signature
        + "\" has an id deviating from standard naming convention by "
        + std::to_string(strct_rep.init_level) + " characters.";
    strct_rep.src_info = SourceInfo(strct.src_info.file,
                                    strct.src_info.line,
                                    strct.src_info.col,
                                    strct.signature);
    icsv::detector::register_report(TAG, strct_rep);

    for (auto& meth : strct.methods) {
      if (!icsv::utils::is_standard_class_func(meth.name, strct.name)) {
        DetectorReport meth_rep;
        meth_rep.init_level = eval->EvaluateMethodName(meth.name);
        meth_rep.level
            = icsv::detector::evaluate_smell(TAG, meth_rep.init_level);
        meth_rep.message = "Method \"" + meth.name
            + "\" has an id deviating from standard naming convention by "
            + std::to_string(meth_rep.init_level) + " characters.";
        meth_rep.src_info = SourceInfo(meth.src_info.file,
                                       meth.src_info.line,
                                       meth.src_info.col,
                                       strct.signature,
                                       meth.signature);
        icsv::detector::register_report(TAG, meth_rep);
      }

      for (auto& arg : meth.args) {
        DetectorReport arg_rep;
        arg_rep.init_level = eval->EvaluateVarName(arg.name);
        arg_rep.level = icsv::detector::evaluate_smell(TAG, arg_rep.init_level);
        arg_rep.message = "Argument \"" + arg.name
            + " has an id deviating from standard naming conventions by "
            + std::to_string(arg_rep.init_level) + " characters.";
        arg_rep.src_info = SourceInfo(meth.src_info.file,
                                      meth.src_info.line,
                                      meth.src_info.col,
                                      strct.signature,
                                      meth.signature);
      }

      for (auto& def : meth.definitions) {
        DetectorReport def_rep;
        def_rep.init_level = eval->EvaluateVarName(def.name);
        def_rep.level = icsv::detector::evaluate_smell(TAG, def_rep.init_level);
        def_rep.message = "Definition \"" + def.name
            + "\" has an id deviating from standard naming convention by "
            + std::to_string(def_rep.init_level) + " characters.";
        def_rep.src_info = SourceInfo(def.src_info.file,
                                      def.src_info.line,
                                      def.src_info.col,
                                      strct.signature,
                                      meth.signature);
        icsv::detector::register_report(TAG, def_rep);
      }
    }

    for (auto& fld : strct.fields) {
      DetectorReport fld_rep;
      fld_rep.init_level = eval->EvaluateVarName(fld.definition.name);
      fld_rep.level   = icsv::detector::evaluate_smell(TAG, fld_rep.init_level);
      fld_rep.message = "Field \"" + fld.signature
          + "\" has an id deviating from standard naming convention by "
          + std::to_string(fld_rep.init_level) + " characters.";
      fld_rep.src_info = SourceInfo(fld.definition.src_info.file,
                                    fld.definition.src_info.line,
                                    fld.definition.src_info.col,
                                    strct.signature);
      icsv::detector::register_report(TAG, fld_rep);
    }
  }
}
