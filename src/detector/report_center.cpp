#include "icsv/detector/report_center.hpp"
#include <assert.h>
#include <ostream>

namespace icsv::detector {

auto
ReportCenter::Get() -> ReportCenter& {
  static ReportCenter singleton;
  return singleton;
}

void
ReportCenter::RegisterReport(const std::string& smell_tag, const Report& rep) {
  m_report_log[smell_tag].push_back(rep);
}

auto
ReportCenter::GetReportsByTag(const std::string& tag) const
    -> const std::list<Report>& {
  if (m_report_log.find(tag) == m_report_log.end())
    std::cout << "Given tag \"" << tag << "\" not found in reports.\n";
  assert(m_report_log.find(tag) != m_report_log.end());

  return m_report_log.find(tag)->second;
}

auto
ReportCenter::GetAllReports(void) const
    -> const std::map<std::string, std::list<Report>>& {
  return m_report_log;
}

void
ReportCenter::ClearReports(void) {
  for (auto& log : m_report_log)
    log.second.clear();
}

}  // namespace icsv::detector
