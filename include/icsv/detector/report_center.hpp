#pragma once

#include "detector_report.hpp"
#include <exception>
#include <list>
#include <map>
#include <string>

#define REGISTER_REPORT(smell_name, rep) \
  icsv::detector::ReportCenter::Get().RegisterReport(smell_name, rep)

namespace icsv::detector {

class NoRegisteredReports : public std::exception {
public:
  NoRegisteredReports()           = default;
  ~NoRegisteredReports() override = default;

  virtual const char* what() const _GLIBCXX_TXN_SAFE_DYN _GLIBCXX_NOTHROW {
    return "There are no reports under the given tag.";
  }
};

class ReportCenter final {
public:
  static auto Get() -> ReportCenter&;

  void RegisterReport(const std::string& smell_tag, const Report& rep);

  auto GetReportsByTag(const std::string& tag) const
      -> const std::list<Report>&;

  auto GetAllReports(void) const
      -> const std::map<std::string, std::list<Report>>&;

  void ClearReports(void);

private:
  std::map<std::string, std::list<Report>> m_report_log;

  ReportCenter()                    = default;
  ReportCenter(const ReportCenter&) = delete;
  ReportCenter(ReportCenter&&)      = delete;
  ~ReportCenter()                   = default;
};

}  // namespace icsv::detector
