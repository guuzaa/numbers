#include "gtest/gtest.h"

#include <numeric>
#include <vector>

#include "int128.hh"

using namespace numbers;

TEST(Int128Test, int128Assignemnt) {
  int a = 10;
  int128 a128 = a;
  ASSERT_EQ(a, a128);
  unsigned int ua = 10;
  int128 ua128 = ua;
  ASSERT_EQ(ua, ua128);

  long b = 1000;
  int128 b128 = b;
  ASSERT_EQ(b, b128);
  unsigned long ub = 1000;
  int128 ub128 = ub;
  ASSERT_EQ(ub, ub128);

  long long c = 1234;
  int128 c128 = c;
  ASSERT_EQ(c, c128);
  unsigned long long uc = 1234;
  int128 uc128 = uc;
  ASSERT_EQ(uc, uc128);
}

TEST(Int128Test, int128Add) {
  int na = 100;
  int nb = 200;
  int nc = na + nb;

  int128 a = na;
  int128 b = nb;
  int128 c = nc;

  ASSERT_EQ(a + b, c);
  ASSERT_EQ(a + b, nc);
  ASSERT_EQ(a + nb, nc);
  ASSERT_EQ(na + b, nc);
  ASSERT_EQ(a++, na++);
  ASSERT_EQ(++a, ++na);

  b += 1;
  nb += 1;
  ASSERT_EQ(b, nb);

  int128 max64 = std::numeric_limits<int64_t>::max();
  uint64_t expected = static_cast<uint64_t>(max64) + 1;
  ASSERT_EQ(max64 + 1, expected);

  int128 umax64 = std::numeric_limits<uint64_t>::max();
  int128 ret = make_int128(1, 0);
  ASSERT_EQ(umax64 + 1, ret);
}

TEST(Int128Test, int128Add1) {
  int64_t sum = 0;
  int128 n = sum;
  for (int64_t i = 0; i < 100000; i++) {
    sum += i;
    n += i;
  }
  ASSERT_EQ(n, sum);
}

TEST(Int128Test, int128Add2) {
  {
    constexpr size_t len = 100'000;
    std::vector<int128> nums(len);
    std::iota(nums.begin(), nums.end(), 0);
    for (size_t i = 0; i < len; i++) {
      ASSERT_EQ(nums[i], i);
    }
  }
  {
    constexpr size_t len = 100;
    std::vector<int128> nums(len);
    std::iota(nums.begin(), nums.end(), 1);
    int128 sum = std::accumulate(nums.begin(), nums.end(), int128(0));
    ASSERT_EQ(sum, 5050);
  }
}

TEST(Int128Test, int128Sub) {
  long na = 100;
  long nb = 2345;
  long nc = na - nb;

  int128 a = na;
  int128 b = nb;
  int128 c = nc;

  ASSERT_EQ(a - b, c);
  ASSERT_EQ(a - b, nc);
  ASSERT_EQ(na - b, nc);
  ASSERT_EQ(a - nb, nc);
  ASSERT_EQ(na - nb, nc);
  ASSERT_EQ(a--, na--);
  ASSERT_EQ(--a, --na);

  b -= 201;
  nb -= 201;
  ASSERT_EQ(b, nb);

  int128 max64 = std::numeric_limits<int64_t>::max();
  uint64_t num = static_cast<uint64_t>(max64) + 1;
  ASSERT_EQ(max64, num - 1);
  int128 two_power_64 = make_int128(1, 0);
  ASSERT_EQ(two_power_64 - 1, int128(std::numeric_limits<uint64_t>::max()));
}

TEST(Int128Test, int128Mul) {
  unsigned long ua = 1234;
  unsigned long ub = 134083;
  unsigned long ret = ua * ub;

  int128 a = ua;
  int128 b = ub;
  ASSERT_EQ(a * b, ret);
  ASSERT_EQ(ua * b, ret);
  ASSERT_EQ(a * ub, ret);
}

// TEST(Int128Test, int128Div) {
//   unsigned long ua = 123458;
//   unsigned long ub = 10;
//   unsigned long ret = ua / ub;

//   int128 a = ua;
//   int128 b = ub;
//   ASSERT_EQ(a / b, ret);
//   ASSERT_EQ(ua / b, ret);
//   ASSERT_EQ(a / ub, ret);
// }