#pragma once

#include "detector_report.hpp"
#include <exception>
#include <list>
#include <string>

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

  auto GetReportsByTag(const std::string& tag) const -> std::list<Report*>;

  auto GetReportList(void) const -> const std::list<Report*>&;

  void SortReportListBy(
      /* functor with which the list is sorted */);  // TODO: sort reports

  void ClearReports(void);

private:
  std::list<Report*> m_report_log;

  ReportCenter()                    = default;
  ReportCenter(const ReportCenter&) = delete;
  ReportCenter(ReportCenter&&)      = delete;
  ~ReportCenter()                   = default;
};

inline void
register_report(const std::string& smell_tag, const Report& rep) {
  ReportCenter::Get().RegisterReport(smell_tag, rep);
}

inline void
clear_reports(void) {
  ReportCenter::Get().ClearReports();
}

}  // namespace icsv::detector
