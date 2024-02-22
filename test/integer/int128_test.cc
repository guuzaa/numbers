#include "gtest/gtest.h"

#include "int128.hh"

using namespace numbers;

TEST(Int128Test, int128Add) {
  int na = 100;
  int nb = 200;
  int nc = na + nb;

  int128 a = na;
  int128 b = nb;
  int128 c = nc;

  ASSERT_EQ(a + b, c);
  ASSERT_EQ(a + b, nc);
  ASSERT_EQ(a++, na++);
  ASSERT_EQ(++a, ++na);

  b += 1;
  nb += 1;
  ASSERT_EQ(b, nb);
}