#pragma once

#include "detector_report.hpp"
#include <exception>
#include <vector>
#include <string>

namespace icsv::detector {

class ReportCenter final {
public:
  using ReportList = std::vector<Report*>;

public:
  static auto Get() -> ReportCenter&;

  void RegisterReport(const std::string& smell_tag, const Report& rep);

  auto GetReportsByTag(const std::string& tag) const -> ReportList;

  auto GetReportList(void) const -> const ReportList&;

  void ClearReports(void);
  void ClearReportsWithTag(const std::string& tag);

  void SeriallizeToFile(const std::string& file_name,
                        int                lvl_min = 0,
                        int                lvl_max = 10);

private:
  ReportList m_report_log;

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
