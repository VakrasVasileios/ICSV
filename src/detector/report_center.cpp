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
  auto* in      = new Report(rep);
  in->smell_tag = smell_tag;
  m_report_log.push_back(in);
}

auto
ReportCenter::GetReportsByTag(const std::string& tag) const
    -> std::list<Report*> {
  std::list<Report*> ret;

  for (auto* rep : m_report_log) {
    if (rep->smell_tag == tag)
      ret.push_back(rep);
  }
  if (ret.empty()) {
    std::cout << "Given tag \"" << tag << "\" not found in reports."
              << std::endl;
    throw NoRegisteredReports();
  }
  assert(!ret.empty());

  return ret;
}

auto
ReportCenter::GetReportList(void) const -> const std::list<Report*>& {
  return m_report_log;
}

void
ReportCenter::SortReportListBy(/* functor with which the list is sorted */) {}

void
ReportCenter::ClearReports(void) {
  for (auto* log : m_report_log)
    delete log;
  m_report_log.clear();
}

}  // namespace icsv::detector
