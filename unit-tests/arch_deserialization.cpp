#include "icsv/detector/arch/architecture_holder.hpp"
#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include <iostream>

namespace icsv::unit_tests {

class Correct_deserialization_check : public testing::Test {
  void SetUp() override {
    detector::arch::ArchitectureHolder::Get().DeserializeArchitecture(
        "/home/vkrs/Documents/ICSV/unit-tests/test.json");
  }
};

TEST_F(Correct_deserialization_check, Checking_for_correct_deserialization) {
  detector::arch::ArchData data
      = detector::arch::ArchitectureHolder::Get().GetArchitecture();
  GTEST_ASSERT_EQ(data.dependencies.size(), 1);

  GTEST_ASSERT_EQ(data.dependencies.front().from, "one");
  GTEST_ASSERT_EQ(data.dependencies.front().to, "two");
  GTEST_ASSERT_EQ(data.dependencies.front().types.name, "dep1");
  GTEST_ASSERT_EQ(data.dependencies.front().types.number, 1);
}

}  // namespace icsv::unit_tests
