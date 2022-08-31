#include "icsv/detector/detector.hpp"
#include <assert.h>
#include <string>

class LongIdentifierDet : public icsv::detector::Detector {
public:
  LongIdentifierDet() : Detector("long_identifier_det") {}
  ~LongIdentifierDet() override = default;

  void SmellDetect(const Json::Value& arch);

private:
  auto GetCleanIdentifier(const std::string& id) -> std::string;
};

LongIdentifierDet* l_id = new LongIdentifierDet();

auto
LongIdentifierDet::GetCleanIdentifier(const std::string& id)
    -> std::string {  // FIXME: using the updated graphs
  assert(!id.empty());
  std::size_t tmp;
  auto        namespace_end
      = (tmp = id.find_last_of(':')) == std::string::npos ? 0 : tmp + 1;
  auto        args_start = (tmp = id.find_first_of('(')) == std::string::npos
             ? id.size() - 1
             : tmp - 1;
  std::string identifier
      = id.substr(namespace_end, args_start - namespace_end + 1);

  return identifier;
}

void
LongIdentifierDet::SmellDetect(const Json::Value& arch) {
  for (auto strct = arch["structures"].begin();
       strct != arch["structures"].end();
       strct++) {

    auto           id_size = GetCleanIdentifier(strct.name()).size();
    DetectorReport strct_rep;
    strct_rep.message = "Structure \"" + strct.name() + "\" has an id of "
        + std::to_string(id_size) + " characters";
    strct_rep.level    = id_size;
    strct_rep.src_info = SourceInfo((*strct)["src_info"]["file"].asString(),
                                    (*strct)["src_info"]["line"].asInt(),
                                    (*strct)["src_info"]["col"].asInt(),
                                    strct.name());
    REGISTER_REPORT("long_identifier", strct_rep);
    std::cout << strct_rep << std::endl;

    for (auto meth = (*strct)["methods"].begin();
         meth != (*strct)["methods"].end();
         meth++) {

      id_size = GetCleanIdentifier(meth.name()).size();
      DetectorReport meth_rep;
      meth_rep.message = "Method \"" + meth.name() + "\" has an id of "
          + std::to_string(id_size) + " characters";
      meth_rep.level    = id_size;
      meth_rep.src_info = SourceInfo((*meth)["src_info"]["file"].asString(),
                                     (*meth)["src_info"]["line"].asInt(),
                                     (*meth)["src_info"]["col"].asInt(),
                                     strct.name(),
                                     meth.name());
      REGISTER_REPORT("long_identifier", meth_rep);

      std::cout << meth_rep << std::endl;

      for (auto arg = (*strct)["methods"]["args"].begin();
           arg != (*strct)["methods"]["args"].end();
           arg++) {

        id_size = GetCleanIdentifier(arg.name()).size();
        DetectorReport arg_rep;
        arg_rep.message = "Argument \"" + arg.name() + "\" of method \""
            + meth.name() + "\" has an id of " + std::to_string(id_size)
            + " characters";
        arg_rep.level    = id_size;
        arg_rep.src_info = SourceInfo((*arg)["src_info"]["file"].asString(),
                                      (*arg)["src_info"]["line"].asInt(),
                                      (*arg)["src_info"]["col"].asInt(),
                                      strct.name(),
                                      meth.name());
        REGISTER_REPORT("long_identifier", arg_rep);

        std::cout << arg_rep << std::endl;
      }

      for (auto def = (*strct)["methods"]["definitions"].begin();
           def != (*strct)["methods"]["definitions"].end();
           def++) {

        id_size = GetCleanIdentifier(def.name()).size();
        DetectorReport def_rep;
        def_rep.message = "Definition \"" + def.name() + "\" in method \""
            + meth.name() + "\" has an id of " + std::to_string(id_size)
            + " characters";
        def_rep.level    = id_size;
        def_rep.src_info = SourceInfo((*def)["src_info"]["file"].asString(),
                                      (*def)["src_info"]["line"].asInt(),
                                      (*def)["src_info"]["col"].asInt(),
                                      strct.name(),
                                      meth.name());
        REGISTER_REPORT("long_identifier", def_rep);
        std::cout << def_rep << std::endl;
      }
    }
    for (auto field = (*strct)["fields"].begin();
         field != (*strct)["fields"].end();
         field++) {

      id_size = GetCleanIdentifier(field.name()).size();
      DetectorReport fld_rep;
      fld_rep.message = "Field \"" + field.name() + "\" of structure \""
          + strct.name() + "\" has an id of " + std::to_string(id_size)
          + " characters";
      fld_rep.level    = id_size;
      fld_rep.src_info = SourceInfo((*field)["src_info"]["file"].asString(),
                                    (*field)["src_info"]["line"].asInt(),
                                    (*field)["src_info"]["col"].asInt(),
                                    strct.name());
      REGISTER_REPORT("long_identifier", fld_rep);
      std::cout << fld_rep << std::endl;
    }
  }
}