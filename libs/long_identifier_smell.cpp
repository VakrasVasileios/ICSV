#include "icsv/detector/detector.hpp"
#include <assert.h>
#include <string>

#define TAG "Excessively long identifier"

class LongIdentifierDet final : public icsv::detector::Detector {
public:
  LongIdentifierDet() : Detector(TAG) {}
  ~LongIdentifierDet() override = default;

  void DetectSmell(const Json::Value& arch);
};

LongIdentifierDet* l_id = new LongIdentifierDet();

void
LongIdentifierDet::DetectSmell(const Json::Value& arch) {
  for (auto strct = arch["structures"].begin();
       strct != arch["structures"].end();
       strct++) {

    auto           id_size = (*strct)["name"].asString().size();
    DetectorReport strct_rep;
    strct_rep.message = "Structure \"" + (*strct)["name"].asString()
        + "\" has an id of " + std::to_string(id_size) + " characters";
    strct_rep.level    = id_size;
    strct_rep.src_info = SourceInfo((*strct)["src_info"]["file"].asString(),
                                    (*strct)["src_info"]["line"].asInt(),
                                    (*strct)["src_info"]["col"].asInt(),
                                    (*strct)["name"].asString());
    REGISTER_REPORT(TAG, strct_rep);

    for (auto meth = (*strct)["methods"].begin();
         meth != (*strct)["methods"].end();
         meth++) {

      id_size = (*meth)["name"].asString().size();
      DetectorReport meth_rep;
      meth_rep.message = "Method \"" + (*meth)["name"].asString()
          + "\" has an id of " + std::to_string(id_size) + " characters";
      meth_rep.level    = id_size;
      meth_rep.src_info = SourceInfo((*meth)["src_info"]["file"].asString(),
                                     (*meth)["src_info"]["line"].asInt(),
                                     (*meth)["src_info"]["col"].asInt(),
                                     (*strct)["name"].asString(),
                                     (*meth)["name"].asString());
      REGISTER_REPORT(TAG, meth_rep);

      for (auto arg = (*strct)["methods"]["args"].begin();
           arg != (*strct)["methods"]["args"].end();
           arg++) {

        id_size = (*arg)["name"].asString().size();
        DetectorReport arg_rep;
        arg_rep.message = "Argument \"" + (*arg)["name"].asString()
            + "\" of method \"" + (*meth)["name"].asString()
            + "\" has an id of " + std::to_string(id_size) + " characters";
        arg_rep.level    = id_size;
        arg_rep.src_info = SourceInfo((*arg)["src_info"]["file"].asString(),
                                      (*arg)["src_info"]["line"].asInt(),
                                      (*arg)["src_info"]["col"].asInt(),
                                      (*strct)["name"].asString(),
                                      (*meth)["name"].asString());
        REGISTER_REPORT(TAG, arg_rep);
      }

      for (auto def = (*strct)["methods"]["definitions"].begin();
           def != (*strct)["methods"]["definitions"].end();
           def++) {

        id_size = (*def)["name"].asString().size();
        DetectorReport def_rep;
        def_rep.message = "Definition \"" + (*def)["name"].asString()
            + "\" in method \"" + (*meth)["name"].asString()
            + "\" has an id of " + std::to_string(id_size) + " characters";
        def_rep.level    = id_size;
        def_rep.src_info = SourceInfo((*def)["src_info"]["file"].asString(),
                                      (*def)["src_info"]["line"].asInt(),
                                      (*def)["src_info"]["col"].asInt(),
                                      (*strct)["name"].asString(),
                                      (*meth)["name"].asString());
        REGISTER_REPORT(TAG, def_rep);
      }
    }
    for (auto field = (*strct)["fields"].begin();
         field != (*strct)["fields"].end();
         field++) {

      id_size = (*field)["name"].asString().size();
      DetectorReport fld_rep;
      fld_rep.message = "Field \"" + (*field)["name"].asString()
          + "\" of structure \"" + (*strct)["name"].asString()
          + "\" has an id of " + std::to_string(id_size) + " characters";
      fld_rep.level    = id_size;
      fld_rep.src_info = SourceInfo((*field)["src_info"]["file"].asString(),
                                    (*field)["src_info"]["line"].asInt(),
                                    (*field)["src_info"]["col"].asInt(),
                                    (*strct)["name"].asString());
      REGISTER_REPORT(TAG, fld_rep);
    }
  }
}