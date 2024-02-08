#include "icsv/detector/report_center.hpp"
#include <assert.h>
#include <ostream>
#include <fstream>

namespace icsv::detector {

auto
ReportCenter::Get() -> ReportCenter& {
  static ReportCenter singleton;
  return singleton;
}

void
ReportCenter::RegisterReport(const char* smell_tag, const Report& rep) {
  auto* in      = new Report(rep);
  in->smell_tag = smell_tag;
  m_report_log.push_back(in);
}

auto
ReportCenter::GetReportsByTag(const std::string& tag) const -> ReportList {
  ReportList ret;

  for (auto* rep : m_report_log) {
    if (rep->smell_tag == tag)
      ret.push_back(rep);
  }
  if (ret.empty()) {
    std::cout << "Given tag \"" << tag << "\" not found in reports."
              << std::endl;
  }

  return ret;
}

auto
ReportCenter::GetReportList(void) const -> const ReportList& {
  return m_report_log;
}

void
ReportCenter::ClearReports(void) {
  for (auto* log : m_report_log) {
    if (log != nullptr) {
      delete log;
      log = nullptr;
    }
  }
  m_report_log.clear();
}

void
ReportCenter::ClearReportsWithTag(const std::string& tag) {
  auto log = m_report_log.begin();
  for (; log != m_report_log.end(); log++) {
    if (*log != nullptr && (*log)->smell_tag == tag) {
      delete *log;
      *log = nullptr;
      log  = m_report_log.erase(log);
    }
  }
}

void
ReportCenter::SeriallizeToFile(const std::string& file_name,
                               int                lvl_min,
                               int                lvl_max) {
  std::ofstream file;
  file.open(file_name);

  if (file.is_open()) {
    file << "{\n\"reports\":[\n";
    bool init = false;

    for (auto* rep : m_report_log) {
      if (lvl_min <= rep->level && rep->level <= lvl_max) {
        if (init)
          file << ",\n";
        file << "{\n";
        file << "\"smell_tag\":\"" << rep->smell_tag << "\",\n";
        file << "\"message\":\"" << rep->message << "\",\n";
        file << "\"level\":" << rep->level << ",\n";
        file << "\"file\":\"" << rep->src_info.file << "\",\n";
        file << "\"line\":" << rep->src_info.line << ",\n";
        file << "\"column\":" << rep->src_info.col << ",\n";
        file << "\"structure\":\"" << rep->src_info.strct << "\",\n";
        file << "\"method\":\"" << rep->src_info.method << "\"\n";
        file << "}";
        init = true;
      }
    }

    file << "\n]\n}";
    file.close();
  } else {
    std::cout << "Could not seriallize reports to given file: " << file_name
              << std::endl;
  }
}

}  // namespace icsv::detector
