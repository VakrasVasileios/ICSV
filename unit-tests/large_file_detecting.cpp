#include "icsv/detector/arch/architecture_holder.hpp"
#include "icsv/detector/detector_manager.hpp"
#include "icsv/detector/evaluation_center.hpp"
#include "icsv/detector/report_center.hpp"
#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include <iostream>

namespace icsv::unit_tests {

class Correct_level_assignment : public testing::Test {
  void SetUp() override {
    detector::arch::ArchitectureHolder::Get().DeserializeArchitecture(
        "/home/vkrs/Documents/ICSV/unit-tests/graph.json");
    detector::EvaluationCenter::Get().DeseriallizeConfig(
        "/home/vkrs/Documents/ICSV/unit-tests/DetectorsConfig.json");
    icsv::detector::DetectorManager::Get().UseDetectors();
  }
};
TEST_F(Correct_level_assignment,
       Checking_correct_scoring_for_large_file_smell) {
  auto& reps
      = icsv::detector::ReportCenter::Get().GetReportsByTag("Large File");
  for (auto& r : reps) {
    GTEST_ASSERT_GE(r.level, 0);
    GTEST_ASSERT_LE(r.level, 10);
  }
}

TEST_F(Correct_level_assignment, Checking_correct_scoring_for_long_id_smell) {
  auto& reps = icsv::detector::ReportCenter::Get().GetReportsByTag(
      "Excessively long identifier");
  for (auto& r : reps) {
    GTEST_ASSERT_GE(r.level, 0);
    GTEST_ASSERT_LE(r.level, 10);
  }
}

}  // namespace icsv::unit_tests