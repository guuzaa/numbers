// Copyright 2017 The Abseil Authors.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//      https://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//
// Modified from abseil-app guuzaa 2024

#include "int128.hh"

#include <random>

#include "gtest/gtest.h"

namespace {
template <typename T>
class Uint128IntegerTraitsTest : public ::testing::Test {};

typedef ::testing::Types<bool, char, signed char, unsigned char, char16_t, char32_t, short, unsigned short, int,
                         unsigned int, long, unsigned long, long long, unsigned long long>
    Uint128IntegerTypes;

TYPED_TEST_SUITE(Uint128IntegerTraitsTest, Uint128IntegerTypes);

TYPED_TEST(Uint128IntegerTraitsTest, ConstructAssignTest) {
  static_assert(std::is_constructible<numbers::uint128, TypeParam>::value,
                "numbers::uint128 must be constructible from TypeParam");
  static_assert(std::is_assignable<numbers::uint128 &, TypeParam>::value,
                "numbers::uint128 must be assignable from TypeParam");
  static_assert(!std::is_assignable<TypeParam &, numbers::uint128>::value,
                "TypeParam must not be assignable from numbers::uint128");
}

template <typename T>
class Uint128FloatTraitsTest : public ::testing::Test {};
typedef ::testing::Types<float, double, long double> Uint128FloatTypes;

TYPED_TEST_SUITE(Uint128FloatTraitsTest, Uint128FloatTypes);

TYPED_TEST(Uint128FloatTraitsTest, ConstructAssignTest) {
  static_assert(std::is_constructible<numbers::uint128, TypeParam>::value,
                "numbers::uint128 must be constructible from TypeParam");
  static_assert(!std::is_assignable<numbers::uint128 &, TypeParam>::value,
                "numbers::uint128 must not be assignable from TypeParam");
  static_assert(!std::is_assignable<TypeParam &, numbers::uint128>::value,
                "TypeParam must not be assignable from numbers::uint128");
}

class Uint128Test : public ::testing::Test {
 protected:
  numbers::uint128 min = std::numeric_limits<numbers::uint128>::min();
  numbers::uint128 max = std::numeric_limits<numbers::uint128>::max();
  numbers::uint128 zero = 0;
  numbers::uint128 one = 1;
  numbers::uint128 one_2arg = numbers::make_uint128(0, 1);
  numbers::uint128 two = 2;
  numbers::uint128 three = 3;
  numbers::uint128 big = numbers::make_uint128(2024, 2);
  numbers::uint128 big_minus_one = numbers::make_uint128(2024, 1);
  numbers::uint128 bigger = numbers::make_uint128(2025, 1);
  numbers::uint128 biggest = std::numeric_limits<numbers::uint128>::max();
  numbers::uint128 high_low = numbers::make_uint128(1, 0);
  numbers::uint128 low_high = numbers::make_uint128(0, std::numeric_limits<uint64_t>::max());

  void SetUp() override {}

  void TearDown() override {}
};

TEST_F(Uint128Test, TrivialTraitsTest) {
  static_assert(std::is_trivially_constructible<numbers::uint128>::value,
                "numbers::uint128 must be trivially constructible");
  static_assert(std::is_trivially_copy_constructible<numbers::uint128>::value,
                "numbers::uint128 must be trivially copy constructible");
  static_assert(std::is_trivially_copy_assignable<numbers::uint128>::value,
                "numbers::uint128 must be trivially copy assignable");
  static_assert(std::is_trivially_destructible<numbers::uint128>::value,
                "numbers::uint128 must be trivially destructible");
}

TEST_F(Uint128Test, Comparsion) {
  EXPECT_LT(one, two);
  EXPECT_GT(two, one);
  EXPECT_LT(one, big);
  EXPECT_LE(one, big);
  EXPECT_EQ(one, one_2arg);
  EXPECT_NE(one, two);
  EXPECT_GT(big, one);
  EXPECT_GE(big, two);
  EXPECT_GE(big, big_minus_one);
  EXPECT_GT(big, big_minus_one);
  EXPECT_LT(big_minus_one, big);
  EXPECT_LE(big_minus_one, big);
  EXPECT_NE(big_minus_one, big);
  EXPECT_LT(big, biggest);
  EXPECT_LE(big, biggest);
  EXPECT_GT(biggest, big);
  EXPECT_GE(biggest, big);

  EXPECT_EQ(zero, +zero);
  EXPECT_EQ(big, +big);
  EXPECT_EQ(two, +two);
  EXPECT_EQ(numbers::uint128_max(), +numbers::uint128_max());
  EXPECT_EQ(big, -(-big));
  EXPECT_EQ(two, -((-one) - 1));
  EXPECT_EQ(numbers::uint128_max(), -one);
  EXPECT_EQ(zero, -zero);

  EXPECT_TRUE(zero == 0);
  EXPECT_FALSE(zero != 0);
  EXPECT_FALSE(one == 0);
  EXPECT_TRUE(one != 0);
  EXPECT_FALSE(high_low == 0);
  EXPECT_TRUE(high_low != 0);

  EXPECT_EQ(numbers::uint128_high64(biggest), std::numeric_limits<uint64_t>::max());
  EXPECT_EQ(numbers::uint128_low64(biggest), std::numeric_limits<uint64_t>::max());
  EXPECT_EQ(numbers::uint128_high64((numbers::uint128(1) << 64) - 1), 0);
  EXPECT_EQ(numbers::uint128_low64((numbers::uint128(1) << 64) - 1), std::numeric_limits<uint64_t>::max());
}

TEST_F(Uint128Test, BitsOperaton) {
  EXPECT_EQ(big, ~~big);
  EXPECT_EQ(biggest, ~~biggest);
  EXPECT_EQ(one, one | one);
  EXPECT_EQ(bigger, bigger | bigger);
  EXPECT_EQ(one, one | zero);
  EXPECT_EQ(one, one & one);
  EXPECT_EQ(big, big & big);
  EXPECT_EQ(zero, one & zero);
  EXPECT_EQ(zero, biggest & zero);
  EXPECT_EQ(zero, big & ~big);
  EXPECT_EQ(zero, biggest & ~biggest);
  EXPECT_EQ(zero, one ^ one);
  EXPECT_EQ(zero, bigger ^ bigger);
  EXPECT_EQ(one, one ^ zero);
  EXPECT_EQ(big_minus_one, big_minus_one ^ zero);

  // Shift operation
  EXPECT_EQ(big, big << 0);
  EXPECT_EQ(bigger, bigger << 0);
  EXPECT_EQ(big, big >> 0);
  EXPECT_EQ(biggest, biggest >> 0);
  EXPECT_GT(big << 1, big);
  EXPECT_LT(big >> 1, big);
  EXPECT_EQ(big, (big << 10) >> 10);
  EXPECT_EQ(bigger, (bigger << 1) >> 1);
  EXPECT_EQ(one, (one << 80) >> 80);
  EXPECT_EQ(zero, (one >> 80) << 80);
  EXPECT_EQ(zero, (one >> 1) << 1);

  // Shift assignment
  numbers::uint128 big_copy = big;
  EXPECT_EQ(big << 0, big_copy <<= 0);
  big_copy = big;
  EXPECT_EQ(big >> 0, big_copy >>= 0);
  big_copy = big;
  EXPECT_EQ(big << 1, big_copy <<= 1);
  big_copy = big;
  EXPECT_EQ(big >> 1, big_copy >>= 1);
  big_copy = big;
  EXPECT_EQ(big << 10, big_copy <<= 10);
  big_copy = big;
  EXPECT_EQ(big >> 10, big_copy >>= 10);
  big_copy = big;
  EXPECT_EQ(big << 64, big_copy <<= 64);
  big_copy = big;
  EXPECT_EQ(big >> 64, big_copy >>= 64);
  big_copy = big;
  EXPECT_EQ(big << 73, big_copy <<= 73);
  big_copy = big;
  EXPECT_EQ(big >> 73, big_copy >>= 73);

  numbers::uint128 test = zero;
  EXPECT_EQ(test |= three, three);
  EXPECT_EQ(test &= one, one);
  EXPECT_EQ(test ^= three, two);
  EXPECT_EQ(test >>= 1, one);
  EXPECT_EQ(test <<= 1, two);
}

TEST_F(Uint128Test, ArithmeticOperation) {
  EXPECT_EQ(zero + one, one);
  EXPECT_EQ(one + one, two);
  EXPECT_EQ(big_minus_one + one, big);
  EXPECT_EQ(one - one, zero);
  EXPECT_EQ(one - zero, one);
  EXPECT_EQ(bigger - bigger, zero);
  EXPECT_EQ(big - one, big_minus_one);
  EXPECT_EQ(big + std::numeric_limits<uint64_t>::max(), bigger);
  EXPECT_EQ(biggest + 1, zero);
  EXPECT_EQ(biggest + one, zero);
  EXPECT_EQ(high_low - one, low_high);
  EXPECT_EQ(low_high + one, high_low);

  numbers::uint128 test = zero;
  EXPECT_EQ(++test, one);
  EXPECT_EQ(test, one);
  EXPECT_EQ(test++, one);
  EXPECT_EQ(test, two);
  EXPECT_EQ(test -= 2, zero);
  EXPECT_EQ(test, zero);
  EXPECT_EQ(test += 2, two);
  EXPECT_EQ(test, two);
  EXPECT_EQ(--test, one);
  EXPECT_EQ(test, one);
  EXPECT_EQ(test--, one);
  EXPECT_EQ(test, zero);

  numbers::uint128 a = 123;
  numbers::uint128 b = 234;
  numbers::uint128 c = a + b;
  EXPECT_EQ(c, 357);
  EXPECT_EQ(min + max, max);
}

TEST_F(Uint128Test, Boolean) {
  EXPECT_TRUE(one);
  EXPECT_FALSE(!one);
  EXPECT_TRUE(!!one);
  EXPECT_TRUE(!!high_low);
  EXPECT_TRUE(!!big);
  EXPECT_TRUE(!!biggest);

  EXPECT_FALSE(zero);
  EXPECT_TRUE(!zero);
  EXPECT_FALSE(!!zero);
}

TEST_F(Uint128Test, Multiplication) {
  numbers::uint128 a, b, c;

  // 0 * 0
  a = 0;
  b = 0;
  c = a * b;
  EXPECT_EQ(0, c);

  // Max carries
  a = numbers::uint128(0) - 1;
  b = numbers::uint128(0) - 1;
  c = a * b;
  EXPECT_EQ(1, c);

  // Self-operation with max carries
  c = numbers::uint128(0) - 1;
  c *= c;
  EXPECT_EQ(1, c);

  // 1-bit * 1-bit
  for (int i = 0; i < 64; ++i) {
    for (int j = 0; j < 64; ++j) {
      a = numbers::uint128(1) << i;
      b = numbers::uint128(1) << j;
      c = a * b;
      EXPECT_EQ(a, c >> j);
      EXPECT_EQ(b, c >> i);
      EXPECT_EQ(numbers::uint128(1) << (i + j), c);
    }
  }

  // Verified with random numbers.
  a = numbers::make_uint128(0xffffeeeeddddcccc, 0xbbbbaaaa99998888);
  b = numbers::make_uint128(0x7777666655554444, 0x3333222211110000);
  c = a * b;
  EXPECT_EQ(numbers::make_uint128(0x530EDA741C71D4C3, 0xBF25975319080000), c);
  EXPECT_EQ(0, c - b * a);
  EXPECT_EQ(a * a - b * b, (a + b) * (a - b));

  a = numbers::make_uint128(0x0123456789abcdef, 0xfedcba9876543210);
  b = numbers::make_uint128(0x02468ace13579bdf, 0xfdb97531eca86420);
  c = a * b;
  EXPECT_EQ(numbers::make_uint128(0x97a87f4f261ba3f2, 0x342d0bbf48948200), c);
  EXPECT_EQ(0, c - b * a);
  EXPECT_EQ(a * a - b * b, (a + b) * (a - b));
}

TEST_F(Uint128Test, DivideAndMod) {
  using std::swap;

  // let a = q * b + r
  numbers::uint128 a, b, q, r;

  a = 0;
  b = 123;
  q = a / b;
  r = a % b;
  EXPECT_EQ(0, q);
  EXPECT_EQ(0, r);

  a = numbers::make_uint128(0x530eda741c71d4c3, 0xbf25975319080000);
  q = numbers::make_uint128(0x4de2cab081, 0x14c34ab4676e4bab);
  b = numbers::uint128(0x1110001);
  r = numbers::uint128(0x3eb455);
  ASSERT_EQ(q * b + r, a);

  numbers::uint128 ret_q, ret_r;
  ret_q = a / b;
  ret_r = a % b;
  EXPECT_EQ(q, ret_q);
  EXPECT_EQ(r, ret_r);

  swap(q, b);
  ret_q = a / b;
  ret_r = a % b;
  EXPECT_EQ(q, ret_q);
  EXPECT_EQ(r, ret_r);
  // Restore
  swap(b, q);

  swap(a, b);
  ret_q = a / b;
  ret_r = a % b;
  EXPECT_EQ(0, ret_q);
  EXPECT_EQ(a, ret_r);

  swap(a, q);
  ret_q = a / b;
  ret_r = a % b;
  EXPECT_EQ(0, ret_q);
  EXPECT_EQ(a, ret_r);
  // Restore
  swap(q, a);
  swap(b, a);

  // Try a large remainder
  b = a / 2 + 1;
  numbers::uint128 expected_r = numbers::make_uint128(0x29876d3a0e38ea61, 0xdf92cba98c83ffff);
  ASSERT_EQ(a / 2 - 1, expected_r);
  ASSERT_EQ(a, b + expected_r);
  ret_q = a / b;
  ret_r = a % b;
  EXPECT_EQ(1, ret_q);
  EXPECT_EQ(expected_r, ret_r);
}

TEST_F(Uint128Test, DivideAndModRandomInputs) {
  const int kNumIters = 1 << 18;
  std::minstd_rand random(testing::UnitTest::GetInstance()->random_seed());
  std::uniform_int_distribution<uint64_t> uniform_uint64;
  for (int i = 0; i < kNumIters; ++i) {
    numbers::uint128 a = numbers::make_uint128(uniform_uint64(random), uniform_uint64(random));
    numbers::uint128 b = numbers::make_uint128(uniform_uint64(random), uniform_uint64(random));
    if (b == 0) {
      continue;
    }
    numbers::uint128 q = a / b;
    numbers::uint128 r = a % b;
    ASSERT_EQ(b * q + r, a);
  }
}

TEST_F(Uint128Test, Constexpr) {
  constexpr numbers::uint128 minus_two = -2;
  EXPECT_EQ(zero, numbers::uint128(0));
  EXPECT_EQ(one, numbers::uint128(1));
  EXPECT_EQ(minus_two, numbers::make_uint128(-1, -2));
}

TEST_F(Uint128Test, NumericLimits) {
  EXPECT_EQ(std::numeric_limits<numbers::uint128>::max(), numbers::uint128_max());
  EXPECT_EQ(std::numeric_limits<numbers::uint128>::min(), numbers::uint128(0));
  EXPECT_EQ(std::numeric_limits<numbers::uint128>::lowest(), numbers::uint128(0));
  EXPECT_EQ(std::numeric_limits<numbers::uint128>::digits, 128);
  EXPECT_EQ(std::numeric_limits<numbers::uint128>::digits10, 38);
  EXPECT_FALSE(std::numeric_limits<numbers::uint128>::is_signed);
  EXPECT_TRUE(std::numeric_limits<numbers::uint128>::is_integer);
  EXPECT_TRUE(std::numeric_limits<numbers::uint128>::is_exact);
  EXPECT_EQ(std::numeric_limits<numbers::uint128>::radix, 2);
  EXPECT_EQ(std::numeric_limits<numbers::uint128>::min_exponent, 0);
  EXPECT_EQ(std::numeric_limits<numbers::uint128>::min_exponent10, 0);
  EXPECT_EQ(std::numeric_limits<numbers::uint128>::max_exponent, 0);
  EXPECT_EQ(std::numeric_limits<numbers::uint128>::max_exponent10, 0);
  EXPECT_FALSE(std::numeric_limits<numbers::uint128>::has_infinity);
  EXPECT_FALSE(std::numeric_limits<numbers::uint128>::has_quiet_NaN);
  EXPECT_FALSE(std::numeric_limits<numbers::uint128>::has_signaling_NaN);
  EXPECT_FALSE(std::numeric_limits<numbers::uint128>::has_denorm);
  EXPECT_FALSE(std::numeric_limits<numbers::uint128>::has_denorm_loss);
  EXPECT_FALSE(std::numeric_limits<numbers::uint128>::is_iec559);
  EXPECT_TRUE(std::numeric_limits<numbers::uint128>::is_bounded);
  EXPECT_TRUE(std::numeric_limits<numbers::uint128>::is_modulo);
  EXPECT_FALSE(std::numeric_limits<numbers::uint128>::traps);
  EXPECT_FALSE(std::numeric_limits<numbers::uint128>::tinyness_before);
  EXPECT_EQ(std::numeric_limits<numbers::uint128>::round_style, std::round_toward_zero);
}

}  // namespace
