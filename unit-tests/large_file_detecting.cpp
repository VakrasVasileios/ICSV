#include "icsv/detector/architecture_holder.hpp"
#include "icsv/detector/detector_manager.hpp"
#include "icsv/detector/report_center.hpp"
#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include <iostream>

namespace icsv::unit_tests {

class Correct_file_line_count_check : public testing::Test {
  void SetUp() override {
    detector::ArchitectureHolder::Get().DeserializeArchitecture(
        "/home/vkrs/Documents/ICSV/unit-tests/graph.json");
  }
};

TEST_F(Correct_file_line_count_check, Checking_for_correct_file_line_count) {
  icsv::detector::DetectorManager::Get().UseDetectors();
  auto& reps
      = icsv::detector::ReportCenter::Get().GetReportsByTag("large_file");
  for (auto& r : reps) {
    GTEST_ASSERT_GE(r.level, 27);
    GTEST_ASSERT_LE(r.level, 158);
  }
}

}  // namespace icsv::unit_tests