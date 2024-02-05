#pragma once

#include <iostream>
#include <assert.h>
#include <string>

namespace icsv::detector {

enum ReportMembers {
  EMPTY_m,
  SMELL_TAG_m,
  MESSAGE_m,
  LEVEL_m,
  INIT_LEVEL_m,
  FILE_m,
  LINE_m,
  COLUMN_m,
  STRUCTURE_m,
  METHOD_m
};

struct SourceInfo {
  std::string file;
  int         line;
  int         col;
  std::string strct;
  std::string method;

  SourceInfo(const std::string& _file   = "",
             int                _line   = 1,
             int                _col    = 1,
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
  std::string smell_tag = "";
  std::string message   = "";
  SourceInfo  src_info;
  float       level      = 0.00f;
  int         init_level = 0;

  auto operator()(ReportMembers memb) -> std::string {
    switch (memb) {
      case SMELL_TAG_m:
        return smell_tag;
      case MESSAGE_m:
        return message;
      case LEVEL_m:
        return std::to_string(static_cast<int>(level));
      case INIT_LEVEL_m:
        return std::to_string(init_level);
      case FILE_m:
        return src_info.file;
      case LINE_m:
        return std::to_string(src_info.line);
      case COLUMN_m:
        return std::to_string(src_info.col);
      case STRUCTURE_m:
        return src_info.strct;
      case METHOD_m:
        return src_info.method;
      case EMPTY_m:
      default:
        return "default :(";
    }
  }
} Report;

inline auto
operator<<(std::ostream& os, const Report& rep) -> std::ostream& {
  return os << "Smell tag: " << rep.smell_tag << '\n'
            << "Message: " << rep.message << '\n'
            << "Source Info:\n"
            << rep.src_info << '\n'
            << "Level: " << rep.level << '\n';
}

}  // namespace icsv::detector