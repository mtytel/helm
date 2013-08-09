#include <limits.h>
#include "test_processor.h"
#include "gtest/gtest.h"

using namespace laf;

TEST(ProcessorTest, NumInputs) {
  TestProcessor p0(0, 0);
  EXPECT_EQ(0, p0.numInputs());
  TestProcessor p1(1, 2);
  EXPECT_EQ(1, p1.numInputs());
  TestProcessor p3(3, 1);
  EXPECT_EQ(3, p3.numInputs());
}

TEST(ProcessorTest, NumOutputs) {
  TestProcessor p0(0, 0);
  EXPECT_EQ(0, p0.numOutputs());
  TestProcessor p1(2, 1);
  EXPECT_EQ(1, p1.numOutputs());
  TestProcessor p3(1, 3);
  EXPECT_EQ(3, p3.numOutputs());
}
