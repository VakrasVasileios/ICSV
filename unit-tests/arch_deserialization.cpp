#include "icsv/detector/architecture_holder.hpp"
#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include <iostream>

namespace icsv::unit_tests {

class Correct_deserialization_check : public testing::Test {
  void SetUp() override {
    detector::ArchitectureHolder::Get().DeserializeArchitecture(
        "/home/vkrs/Documents/ICSV/unit-tests/test.json");
  }
};

TEST_F(Correct_deserialization_check, Checking_for_correct_deserialization) {
  Json::Value root = detector::ArchitectureHolder::Get().GetArchitecture();
  GTEST_ASSERT_EQ(root["dependencies"].size(), 3);
  GTEST_ASSERT_EQ(root["dependencies"][0]["dep1"], "one");
  GTEST_ASSERT_EQ(root["dependencies"][1]["dep2"], "two");
  GTEST_ASSERT_EQ(root["dependencies"][2]["dep3"], "three");
}

}  // namespace icsv::unit_tests
