#include "gtest/gtest.h"

#include "i128.hh"

TEST(Int128Test, Integer128Add) {
  auto a = 100_i128;
  auto b = 200_i128;
  auto c = 300_i128;
  ASSERT_EQ(a + b, c);
}