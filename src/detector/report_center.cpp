#include "icsv/detector/report_center.hpp"
#include <ostream>

namespace detector {

auto
operator<<(std::ostream& os, const Report& rep) -> std::ostream& {
  os << "Message: " << rep.message << '\n';
  os << "Source: " << rep.source << '\n';
  os << "Level: " << rep.level << '\n';
  return os;
}

auto
ReportCenter::Get() -> ReportCenter& {
  static ReportCenter singleton;
  return singleton;
}

void
ReportCenter::RegisterReport(const std::string& smell_tag, const Report& rep) {
  m_report_log[smell_tag].push_back(rep);
}

void
ReportCenter::ClearReports(void) {
  for (auto& log : m_report_log)
    log.second.clear();
}

}  // namespace detector
