#pragma once

#include <map>
#include <cstring>
#include <string>

namespace ICSVapp {

struct SmellIntesity {
  float  level   = 0.0f;
  float  average = 0.0f;
  size_t level_c[10];
  size_t count = 0;

  SmellIntesity() { std::memset(level_c, 0, 10 * sizeof(size_t)); }
};

class SmellStatistics final {
public:
  static auto Get(void) -> SmellStatistics&;

  void AddSmellIntensity(const std::string& tag, float lvl);
  void AddFileIntensity(const std::string& file, float lvl);

  void ClearStats(void);

  void DisplayGui(void);

private:
  using StatsMap = std::map<std::string, SmellIntesity>;

private:
  float m_overall_int = 0.0f;

  StatsMap m_smell_int_map;
  StatsMap m_file_int_map;

  SmellStatistics()                       = default;
  SmellStatistics(const SmellStatistics&) = delete;
  SmellStatistics(SmellStatistics&&)      = delete;
  ~SmellStatistics()                      = default;
};

}  // namespace ICSVapp
