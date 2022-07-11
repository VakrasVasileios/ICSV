#pragma once

#include <list>
#include <map>
#include <string>

namespace detector {

typedef struct DetectorReport {
  std::string message = "";
  std::string source  = "";
  int         level   = -1;
} Report;

class ReportCenter final {
public:
  static auto Get() -> ReportCenter&;

  void RegisterReport(const std::string& smell_tag, const Report& rep);

  void ClearReports(void);

private:
  ReportCenter()                    = default;
  ReportCenter(const ReportCenter&) = delete;
  ReportCenter(ReportCenter&&)      = delete;
  ~ReportCenter()                   = default;

  std::map<std::string, std::list<Report>> m_report_log;
};

}  // namespace detector
