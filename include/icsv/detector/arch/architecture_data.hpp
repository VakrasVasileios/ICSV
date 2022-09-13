#pragma once

#include <list>
#include <string>

namespace icsv::detector::arch {

struct Structure;
struct Dependency;
struct Method;
struct Field;
struct SrcInfo;
struct Definition;

using Structures   = std::list<Structure>;
using Dependencies = std::list<Dependency>;
using Sources      = std::list<std::string>;
using Bases        = std::list<std::string>;
using Fields       = std::list<Field>;
using Methods      = std::list<Method>;
using Friends      = std::list<std::string>;
using Contains     = std::list<std::string>;
using TemplateArgs = std::list<std::string>;
using Definitions  = std::list<Definition>;

struct SrcInfo {
  unsigned    col;
  std::string file;
  unsigned    line;
};

struct Definition {
  std::string full_type;
  std::string name;
  SrcInfo     src_info;
  std::string type;
};

struct Field {
  std::string access;
  std::string signature;
  Definition  definition;
};

struct ArchData {
  Structures   structures;
  Dependencies dependencies;
  Sources      sources;
};

struct Structure {
  std::string  signature;
  Bases        bases;
  Contains     contains;
  Fields       fields;
  Friends      friends;
  Methods      methods;
  std::string  name;
  std::string  name_space;
  std::string  nested_parent;
  SrcInfo      src_info;
  std::string  structure_type;
  TemplateArgs template_args;
  std::string  template_parent;
};

struct Dependency {
  struct DepType {
    std::string name;
    unsigned    number;
  };

  std::string from;
  std::string to;
  DepType     types;
};

struct Method {
  using Args = std::list<Definition>;
  std::string  signature;
  std::string  access;
  Args         args;
  unsigned     branches;
  Definitions  definitions;
  unsigned     lines;
  unsigned     literals;
  unsigned     loops;
  unsigned     max_scope;
  std::string  method_type;
  std::string  name;
  std::string  ret_type;
  SrcInfo      src_info;
  unsigned     statements;
  TemplateArgs template_args;
  bool         is_virtual;
};

}  // namespace icsv::detector::arch
