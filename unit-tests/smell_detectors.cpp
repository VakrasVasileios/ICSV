#include "icsv/detector/arch/architecture_holder.hpp"
#include "icsv/detector/detector_manager.hpp"
#include "icsv/detector/evaluation_center.hpp"
#include "icsv/detector/report_center.hpp"
#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include <iostream>

#define ASSERT_REPORTS(tag)                                               \
  try {                                                                   \
    auto reps = icsv::detector::ReportCenter::Get().GetReportsByTag(tag); \
    for (auto* r : reps) {                                                \
      GTEST_ASSERT_GE(r->level, 0);                                       \
      GTEST_ASSERT_LE(r->level, 10);                                      \
    }                                                                     \
  } catch (icsv::detector::NoRegisteredReports & e) {                     \
    std::cout << e.what() << std::endl;                                   \
    GTEST_ASSERT_TRUE(true);                                              \
  }

namespace icsv::unit_tests {

class Correct_level_assignment : public testing::Test {
  void SetUp() override {
    icsv::detector::arch::ArchitectureHolder::Get().DeserializeArchitecture(
        "/home/vkrs/Documents/ICSV/unit-tests/graph.json");
    icsv::detector::EvaluationCenter::Get().DeseriallizeConfig(
        "/home/vkrs/Documents/ICSV/unit-tests/DetectorsConfig.json");
    icsv::detector::DetectorManager::Get().UseDetectors();
  }
};

TEST_F(Correct_level_assignment,
       Checking_correct_scoring_for_large_file_smell) {
  ASSERT_REPORTS("Large File");
}

TEST_F(Correct_level_assignment, Checking_correct_scoring_for_long_id_smell) {
  ASSERT_REPORTS("Excessively long identifier");
}
TEST_F(Correct_level_assignment, Checking_correct_scoring_for_nested_scopes) {
  ASSERT_REPORTS("Nested Scopes");
}

TEST_F(Correct_level_assignment,
       Checking_correct_scoring_for_dependency_count) {
  ASSERT_REPORTS("Many Dependencies");
}

TEST_F(Correct_level_assignment,
       Checking_correct_scoring_for_function_overload_count) {
  ASSERT_REPORTS("Excessive function overloading");
}

TEST_F(Correct_level_assignment,
       Checking_correct_scoring_for_non_virtual_base_destructor) {
  ASSERT_REPORTS("Non virtual base destructor");
}

TEST_F(Correct_level_assignment,
       Checking_correct_scoring_for_method_argument_count) {
  ASSERT_REPORTS("Too many parameters");
}

TEST_F(Correct_level_assignment,
       Checking_correct_scoring_for_method_literal_count) {
  ASSERT_REPORTS("Too many function literals");
}

TEST_F(Correct_level_assignment,
       Checking_correct_scoring_for_method_definition_count) {
  ASSERT_REPORTS("Too many locals");
}

TEST_F(Correct_level_assignment,
       Checking_correct_scoring_for_redundant_protected_accesses) {
  ASSERT_REPORTS("Redundant protected access");
}

TEST_F(Correct_level_assignment,
       Checking_correct_scoring_for_naming_conventions) {
  ASSERT_REPORTS("Naming conventions");
}

}  // namespace icsv::unit_tests