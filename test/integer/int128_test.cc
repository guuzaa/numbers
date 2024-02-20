#include "gtest/gtest.h"

#include "int128.hh"

using namespace numbers;

TEST(Int128Test, int128Add) {
  uint128 a = 100;
  uint128 b = 200;
  uint128 c = 300;
  ASSERT_EQ(a + b, c);
}