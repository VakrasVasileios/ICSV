#include "icsv/detector/arch/architecture_holder.hpp"
#include "icsv/detector/detector_manager.hpp"
#include "icsv/detector/evaluation_center.hpp"
#include "icsv/detector/report_center.hpp"
#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include <iostream>
#include <string>
#include <tuple>

#define ASSERT_REPORTS(tag)                                             \
  auto reps = icsv::detector::ReportCenter::Get().GetReportsByTag(tag); \
  if (!reps.empty()) {                                                  \
    for (auto* r : reps) {                                              \
      GTEST_ASSERT_GE(r->level, 0);                                     \
      GTEST_ASSERT_LE(r->level, 10);                                    \
    }                                                                   \
  } else {                                                              \
    GTEST_ASSERT_TRUE(true);                                            \
  }

namespace icsv::unit_tests {

class Correct_level_assignment : public testing::Test {
  void SetUp() override {
    static bool init = false;
    if (!init) {
      icsv::detector::arch::ArchitectureHolder::Get().DeserializeArchitecture(
          "/home/vkrs/Documents/ICSV/unit-tests/graph.json");
      icsv::detector::EvaluationCenter::Get().DeseriallizeConfig(
          "/home/vkrs/Documents/ICSV/unit-tests/DetectorsConfig.json");
      icsv::detector::DetectorManager::Get().UseDetectors();
      init = true;
    }
  }
};

class Correctly_Sorted_Entt_List : public Correct_level_assignment {};

class Always_Greater_Equal_to_Zero : public testing::Test {};

class String_Literal_Template : public testing::Test {};

void
always_GE_zero(int& num) {
  num = num * (num >= 0);
}

enum MType { RANGE, REGEX, BOOL };

template <MType T, typename... Targs>
auto
some(Targs... args) -> std::tuple<char, Targs...> {
  return { '0', args... };
}

template <>
auto
some<RANGE, std::string, int>(std::string s, int i)
    -> std::tuple<char, std::string, int> {
  return { 'R', s, i };
}

template <>
auto
some<REGEX, std::string, std::string>(std::string s1, std::string s2)
    -> std::tuple<char, std::string, std::string> {
  return { 'X', s1, s2 };
}

template <>
auto
some<BOOL, std::string, bool>(std::string s, bool b)
    -> std::tuple<char, std::string, bool> {
  return { 'B', s, b };
}

TEST_F(String_Literal_Template, Checking_string_literal_template) {
  GTEST_ASSERT_TRUE(some<RANGE>(std::string("tag"), 10)
                    == std::make_tuple('R', std::string("tag"), 10));
  GTEST_ASSERT_TRUE(
      some<REGEX>(std::string("tag"), std::string("regex"))
      == std::make_tuple('X', std::string("tag"), std::string("regex")));
  GTEST_ASSERT_TRUE(some<BOOL>(std::string("tag"), true)
                    == std::make_tuple('B', std::string("tag"), true));
}

TEST_F(Correctly_Sorted_Entt_List, Checking_if_entt_list_is_correctly_sorted) {
  using namespace icsv::detector;
  auto d_lst = ReportCenter::Get().GetReportList();
  auto _sort = [&](DetectorReport* e1, DetectorReport* e2) {
    return std::string(e1->smell_tag + std::to_string(e1->level))
        < std::string(e2->smell_tag + std::to_string(e2->level));
  };

  std::stable_sort(d_lst.begin(), d_lst.end(), _sort);

  std::string tag = d_lst.front()->smell_tag;
  int         lvl = d_lst.front()->level;

  for (auto* r : d_lst)
    std::cout << *r << '\n';

  for (auto* rep : d_lst) {
    GTEST_ASSERT_LE(tag, rep->smell_tag);
    if (rep->smell_tag == tag) {
      GTEST_ASSERT_LE(lvl, rep->level);
    }
    tag = rep->smell_tag;
    lvl = rep->level;
  }
}

TEST_F(Always_Greater_Equal_to_Zero, Checking_always_greater_equal_to_zero) {
  int num = 10;
  always_GE_zero(num);
  GTEST_ASSERT_TRUE(num >= 0);
  num = 30022;
  always_GE_zero(num);
  GTEST_ASSERT_TRUE(num >= 0);
  num = -1;
  always_GE_zero(num);
  GTEST_ASSERT_TRUE(num == 0);
  num = -109743;
  always_GE_zero(num);
  GTEST_ASSERT_TRUE(num == 0);
}

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