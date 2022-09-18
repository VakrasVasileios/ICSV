#pragma once

#include <iostream>
#include <string>

namespace icsv::detector {

struct SourceInfo {
  std::string file;
  int         line;
  int         col;
  std::string strct;
  std::string method;

  SourceInfo(const std::string& _file   = "",
             int                _line   = -1,
             int                _col    = -1,
             const std::string& _strct  = "",
             const std::string& _method = "")
      : file(_file), line(_line), col(_col), strct(_strct), method(_method) {}
};

inline auto
operator<<(std::ostream& os, const SourceInfo& s) -> std::ostream& {
  return os << "\tFile: " << s.file << '\n'
            << "\tLine: " << s.line << '\n'
            << "\tColumn: " << s.col << '\n'
            << "\tStructure: " << s.strct << '\n'
            << "\tMethod: " << s.method;
}

typedef struct DetectorReport {
  std::string message = "";
  SourceInfo  src_info;
  int         level      = -1;
  int         init_level = -1;
} Report;

inline auto
operator<<(std::ostream& os, const Report& rep) -> std::ostream& {
  return os << "Message: " << rep.message << '\n'
            << "Source Info:\n"
            << rep.src_info << '\n'
            << "Level: " << rep.level << '\n';
}

}  // namespace icsv::detector