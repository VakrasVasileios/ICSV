#include "icsv/detector/arch/architecture_holder.hpp"
#include <assert.h>
#include <fstream>
#include <json/json.h>

namespace icsv::detector::arch {

auto
ArchitectureHolder::Get() -> ArchitectureHolder& {
  static ArchitectureHolder singleton;
  return singleton;
}

void
ArchitectureHolder::DeserializeArchitecture(const std::string& path) {
  m_data.dependencies.clear();
  m_data.sources.clear();
  m_data.structures.clear();

  Json::Value   json;
  std::ifstream file(path);
  assert(file.is_open());
  file >> json;
  assert(json.isObject());
  file.close();

  for (auto d : json["dependencies"]) {
    Dependency dep;
    dep.from = d["from"].asString();
    dep.to   = d["to"].asString();

    for (auto it = d["types"].begin(); it != d["types"].end(); it++) {
      Dependency::DepType t;
      t.name   = it.name();
      t.number = (*it)[it.name()].asUInt();
      dep.types.emplace_back(t);
    }

    m_data.dependencies.emplace_back(dep);
  }
  for (auto src : json["sources"]) {
    m_data.sources.emplace_back(src.asString());
  }
  for (auto it = json["structures"].begin(); it != json["structures"].end();
       it++) {
    Structure s;
    s.signature  = it.name();
    s.name       = (*it)["name"].asString();
    s.name_space = (*it)["namespace"].asString();

    s.nested_parent = (*it)["nested_parent"].isNull()
        ? "null"
        : (*it)["nested_parent"].asString();

    s.template_parent = (*it)["template_parent"].isNull()
        ? "null"
        : (*it)["template_parent"].asString();

    s.structure_type = (*it)["structure_type"].asString();
    s.src_info.col   = (*it)["src_info"]["col"].asUInt();
    s.src_info.file  = (*it)["src_info"]["file"].asString();
    s.src_info.line  = (*it)["src_info"]["line"].asUInt();

    for (auto base : (*it)["bases"])
      s.bases.emplace_back(base.asString());

    for (auto cont : (*it)["contains"])
      s.contains.emplace_back(cont.asString());

    for (auto fr : (*it)["friends"])
      s.friends.emplace_back(fr.asString());

    for (auto t_arg : (*it)["template_args"])
      s.template_args.emplace_back(t_arg.asString());

    for (auto meth = (*it)["methods"].begin(); meth != (*it)["methods"].end();
         meth++) {
      Method m;
      m.signature     = meth.name();
      m.name          = (*meth)["name"].asString();
      m.access        = (*meth)["access"].asString();
      m.branches      = (*meth)["branches"].asUInt();
      m.lines         = (*meth)["lines"].asUInt();
      m.literals      = (*meth)["literals"].asUInt();
      m.loops         = (*meth)["loops"].asUInt();
      m.max_scope     = (*meth)["max_scope"].asUInt();
      m.method_type   = (*meth)["method_type"].asString();
      m.ret_type      = (*meth)["ret_type"].asString();
      m.is_virtual    = (*meth)["virtual"].asBool();
      m.statements    = (*meth)["statements"].asUInt();
      m.src_info.col  = (*meth)["src_info"]["col"].asUInt();
      m.src_info.line = (*meth)["src_info"]["line"].asUInt();
      m.src_info.file = (*meth)["src_info"]["file"].asString();

      for (auto t_arg : (*meth)["template_args"])
        m.template_args.emplace_back(t_arg.asString());

      for (auto df : (*meth)["definitions"]) {
        Definition def;
        def.name         = df["name"].asString();
        def.full_type    = df["full_type"].asString();
        def.type         = df["type"].isNull() ? "null" : df["type"].asString();
        def.src_info.col = df["src_info"]["col"].asUInt();
        def.src_info.line = df["src_info"]["line"].asUInt();
        def.src_info.file = df["src_info"]["file"].asString();
        m.definitions.emplace_back(def);
      }

      for (auto arg : (*meth)["args"]) {
        Definition def;
        def.name      = arg["name"].asString();
        def.full_type = arg["full_type"].asString();
        def.type      = arg["type"].isNull() ? "null" : arg["type"].asString();
        def.src_info.col  = arg["src_info"]["col"].asUInt();
        def.src_info.line = arg["src_info"]["line"].asUInt();
        def.src_info.file = arg["src_info"]["file"].asString();

        m.args.emplace_back(def);
      }
    }  // for auto meth

    for (auto f = (*it)["fields"].begin(); f != (*it)["fields"].end(); f++) {
      Field field;
      field.signature            = f.name();
      field.access               = (*f)["access"].asString();
      field.definition.full_type = (*f)["full_type"].asString();
      field.definition.name      = (*f)["name"].asString();
      field.definition.type
          = (*f)["type"].isNull() ? "null" : (*f)["type"].asString();
      field.definition.src_info.col  = (*f)["src_info"]["col"].asUInt();
      field.definition.src_info.line = (*f)["src_info"]["line"].asUInt();
      field.definition.src_info.file = (*f)["src_info"]["file"].asString();

      s.fields.emplace_back(field);
    }

    m_data.dependencies.emplace_back(s);
  }
}

}  // namespace icsv::detector::arch