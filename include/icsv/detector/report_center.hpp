#pragma once

#include "detector_report.hpp"
#include <list>
#include <map>
#include <string>

#define REGISTER_REPORT(smell_name, rep) \
  icsv::detector::ReportCenter::Get().RegisterReport(smell_name, rep)

namespace icsv::detector {

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

}  // namespace icsv::detector
