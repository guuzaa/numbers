#include "gtest/gtest.h"

#include "uinteger.hh"

TEST(u128OverflowTest, AddThrow) {
  numbers::u128 max = numbers::u128::MAX;
  EXPECT_THROW(max + 1, std::runtime_error);
  EXPECT_THROW(1 + max, std::runtime_error);
  numbers::u128 min = numbers::u128::MIN;
  EXPECT_NO_THROW(min + max);

  EXPECT_THROW(max++, std::runtime_error);
  EXPECT_THROW(++max, std::runtime_error);

  numbers::u128 a = 123;
  numbers::u128 b = 234;
  EXPECT_NO_THROW(a + b);
  a = numbers::make_uint128(0x12, 0x345);
  b = numbers::make_uint128(0x34, 0x567);
  EXPECT_NO_THROW(a + b);
}

TEST(u128OverflowTest, CheckedAdd) {
  numbers::u128 max = numbers::u128::MAX;
  auto ret = max.checked_add(10);
  EXPECT_EQ(ret, std::nullopt);

  ret = max.checked_add(0);
  EXPECT_EQ(ret.value(), max);

  numbers::u128 min = numbers::u128::MIN;
  ret = min.checked_add(max);
  EXPECT_EQ(ret.value(), max);
}

TEST(u128OverflowTest, SaturatingAdd) {
  numbers::u128 max = numbers::u128::MAX;
  auto ret = max.saturating_add(10);
  EXPECT_EQ(ret, max);

  numbers::u128 max_minus_one = max - 1;
  ret = max_minus_one.saturating_add(1);
  EXPECT_EQ(ret, max);

  numbers::u128 a = 456;
  numbers::u128 b = 789;
  ret = a.saturating_add(b);
  EXPECT_EQ(ret, 1245);
}

TEST(u128OverflowTest, OverflowingAdd) {
  numbers::u128 max = numbers::u128::MAX;
  numbers::u128 min = numbers::u128::MIN;
  auto [ret, flag] = max.overflowing_add(10);
  EXPECT_TRUE(flag);
  EXPECT_EQ(min + 9, ret);

  numbers::u128 a = 678;
  numbers::u128 b = 123;
  std::tie(ret, flag) = a.overflowing_add(b);
  EXPECT_FALSE(flag);
  EXPECT_EQ(801, ret);
}

TEST(u128OverflowTest, WrappingAdd) {
  numbers::u128 max = numbers::u128::MAX;
  numbers::u128 min = numbers::u128::MIN;
  auto ret = max.wrapping_add(10);
  EXPECT_EQ(min + 9, ret);

  numbers::u128 a = 678;
  numbers::u128 b = 1234;
  ret = a.wrapping_add(b);
  EXPECT_EQ(1912, ret);
}

TEST(u128OverflowTest, SubThrow) {
  numbers::u128 min = numbers::u128::MIN;
  numbers::u128 max = numbers::u128::MAX;

  for (int n = 1; n < 1000; n += 2) {
    numbers::u128 num = n;
    EXPECT_THROW(min - num, std::runtime_error);
  }

  EXPECT_THROW(--min, std::runtime_error);
  EXPECT_THROW(min--, std::runtime_error);
  EXPECT_THROW(min -= 34, std::runtime_error);

  EXPECT_EQ(max - max, 0);
  EXPECT_EQ(min - min, 0);
  EXPECT_EQ(max -= max, 0);
  EXPECT_EQ(min -= min, 0);

  numbers::u128 a = 123;
  EXPECT_EQ(123 - a, 0);
}

TEST(u128OverflowTest, CheckedSub) {
  numbers::u128 min = numbers::u128::MIN;
  numbers::u128 max = numbers::u128::MAX;

  EXPECT_EQ(max.checked_sub(max), 0);
  EXPECT_EQ(min.checked_sub(min), 0);

  for (int n = 1; n < 1000; n++) {
    numbers::u128 num = n;
    auto ret = min.checked_sub(num);
    EXPECT_EQ(ret, std::nullopt);
  }

  numbers::u128 a = 123;
  numbers::u128 b = 234;
  numbers::u128 c = 100;
  auto ret = b.checked_sub(a);
  EXPECT_EQ(ret, b - a);
  ret = c.checked_sub(a);
  EXPECT_EQ(ret, std::nullopt);
}

TEST(u128OverflowTest, SaturatingSub) {
  numbers::u128 min = numbers::u128::MIN;
  numbers::u128 max = numbers::u128::MAX;
  for (int n = 1; n < 1000; n++) {
    numbers::u128 num = n;
    auto ret = min.saturating_sub(num);
    EXPECT_EQ(ret, min);
  }

  numbers::u128 a = numbers::make_uint128(123, 0);
  numbers::u128 b = numbers::make_uint128(234, 0);
  numbers::u128 c = numbers::make_uint128(100, 0);
  auto ret = b.saturating_sub(a);
  EXPECT_EQ(ret, b - a);
  ret = c.saturating_sub(a);
  EXPECT_EQ(ret, min);
}

TEST(u128OverflowTest, OverflowingAndWrappingSub) {
  numbers::u128 min = numbers::u128::MIN;
  numbers::u128 max = numbers::u128::MAX;

  for (int n = 1; n < 1000; n++) {
    numbers::u128 num = n;
    auto [ret, flag] = min.overflowing_sub(num);
    EXPECT_TRUE(flag);
    EXPECT_EQ(ret, max - num + 1);
    EXPECT_EQ(ret, min.wrapping_sub(num));
  }

  numbers::u128 a = 123;
  numbers::u128 b = 567;
  numbers::u128 c = 100;
  auto [ret, flag] = b.overflowing_sub(a);
  EXPECT_FALSE(flag);
  EXPECT_EQ(ret, b - a);
  EXPECT_EQ(ret, b.wrapping_sub(a));
  std::tie(ret, flag) = c.overflowing_sub(a);
  EXPECT_TRUE(flag);
  EXPECT_EQ(ret, c.wrapping_sub(a));
}

TEST(u128OverflowTest, MulThrow) {
  numbers::u128 min = numbers::u128::MIN;
  numbers::u128 max = numbers::u128::MAX;
  EXPECT_THROW(max * 12, std::runtime_error);
  EXPECT_NO_THROW(max * 1);
  EXPECT_NO_THROW(min * 345);

  numbers::u128 large = numbers::make_uint128(1, 0);
  numbers::u128 larger = numbers::make_uint128(2, 0);
  EXPECT_NO_THROW(large * 2);
  EXPECT_EQ(large * 2, larger);
  EXPECT_THROW(large * larger, std::runtime_error);
}

TEST(u128OverflowTest, CheckedMul) {
  numbers::u128 min = numbers::u128::MIN;
  numbers::u128 max = numbers::u128::MAX;
  auto ret = max.checked_mul(123);
  EXPECT_EQ(ret, std::nullopt);
  ret = max.checked_mul(456);
  EXPECT_EQ(ret, std::nullopt);
  ret = min.checked_mul(34);
  EXPECT_EQ(ret, min);

  numbers::u128 large = numbers::make_uint128(1, 0);
  numbers::u128 larger = numbers::make_uint128(2, 0);
  ret = large.checked_mul(2);
  EXPECT_EQ(ret, larger);
  ret = large.checked_mul(larger);
  EXPECT_EQ(ret, std::nullopt);
}

TEST(u128OverflowTest, SaturatingMul) {
  numbers::u128 min = numbers::u128::MIN;
  numbers::u128 max = numbers::u128::MAX;
  auto ret = max.saturating_mul(123);
  EXPECT_EQ(ret, max);
  ret = max.saturating_mul(445);
  EXPECT_EQ(ret, max);
  ret = min.saturating_mul(34);
  EXPECT_EQ(ret, min);

  numbers::u128 large = numbers::make_uint128(1, 0);
  numbers::u128 larger = numbers::make_uint128(2, 0);
  ret = large.saturating_mul(2);
  EXPECT_EQ(ret, larger);
  ret = large.saturating_mul(larger);
  EXPECT_EQ(ret, max);
}

TEST(u128OverflowTest, OverflowingMul) {
  numbers::u128 min = numbers::u128::MIN;
  numbers::u128 max = numbers::u128::MAX;
  auto [ret, flag] = max.overflowing_mul(123);
  EXPECT_TRUE(flag);

  std::tie(ret, flag) = min.overflowing_mul(34);
  EXPECT_FALSE(flag);

  numbers::u128 large = numbers::make_uint128(1, 0);
  numbers::u128 larger = numbers::make_uint128(2, 0);
  std::tie(ret, flag) = large.overflowing_mul(2);
  EXPECT_FALSE(flag);
  EXPECT_EQ(ret, larger);
  std::tie(ret, flag) = large.overflowing_mul(larger);
  EXPECT_TRUE(flag);
}

TEST(u128OverflowTest, WrappingMul) {
  numbers::u128 min = numbers::u128::MIN;
  numbers::u128 max = numbers::u128::MAX;
  numbers::u128 a = 345;
  numbers::u128 b = 12;

  auto [ret, flag] = max.overflowing_mul(a);
  EXPECT_TRUE(flag);
  EXPECT_EQ(ret, max.wrapping_mul(a));

  std::tie(ret, flag) = min.overflowing_mul(b);
  EXPECT_FALSE(flag);
  EXPECT_EQ(ret, min.wrapping_mul(b));

  EXPECT_EQ(a.wrapping_mul(b), a * b);  // +, +
  EXPECT_EQ(b.wrapping_mul(a), b * a);

  numbers::u128 large = numbers::make_uint128(1, 0);
  numbers::u128 larger = numbers::make_uint128(2, 0);
  std::tie(ret, flag) = large.overflowing_mul(2);
  EXPECT_FALSE(flag);
  EXPECT_EQ(ret, large.wrapping_mul(2));
  std::tie(ret, flag) = large.overflowing_mul(larger);
  EXPECT_TRUE(flag);
  EXPECT_EQ(ret, large.wrapping_mul(larger));
}

TEST(u128OverflowTest, DivThrow) {
  numbers::u128 max = numbers::u128::MAX;
  EXPECT_NO_THROW(max / 100);

  numbers::u128 a = 123;
  numbers::u128 b = 456;
  numbers::u128 c = 789;

  EXPECT_NO_THROW(a / b);
  EXPECT_NO_THROW(b / a);

  EXPECT_NO_THROW(a / c);
  EXPECT_NO_THROW(c / a);

  EXPECT_NO_THROW(c / b);
  EXPECT_NO_THROW(b / c);

  numbers::u128 zero = 0;
  EXPECT_DEATH(a / zero, "");
}

TEST(u128OverflowTest, CheckedDiv) {
  numbers::u128 a = 321;
  numbers::u128 b = 564;
  numbers::u128 c = 798;
  EXPECT_EQ(a.checked_div(b).value(), a / b);
  EXPECT_EQ(b.checked_div(a).value(), b / a);

  EXPECT_EQ(a.checked_div(c).value(), a / c);
  EXPECT_EQ(c.checked_div(b).value(), c / b);

  EXPECT_EQ(c.checked_div(a).value(), c / a);

  numbers::u128 zero = 0;
  EXPECT_DEATH(a.checked_div(zero), "");
}

TEST(u128OverflowTest, SaturatingDiv) {
  numbers::u128 a = 21;
  numbers::u128 b = 56;
  numbers::u128 c = 78;
  EXPECT_EQ(a.saturating_div(b), a / b);
  EXPECT_EQ(b.saturating_div(a), b / a);

  EXPECT_EQ(a.saturating_div(c), a / c);
  EXPECT_EQ(c.saturating_div(b), c / b);
  EXPECT_EQ(c.saturating_div(a), c / a);

  numbers::u128 zero = 0;
  EXPECT_DEATH(a.saturating_div(zero), "");
}

TEST(u128OverflowTest, OverflowingDiv) {
  numbers::u128 a = 21;
  numbers::u128 b = 56;
  numbers::u128 c = 789;

  auto [ret, flag] = a.overflowing_div(b);
  EXPECT_FALSE(flag);
  EXPECT_EQ(ret, a / b);

  std::tie(ret, flag) = b.overflowing_div(a);
  EXPECT_FALSE(flag);
  EXPECT_EQ(ret, b / a);

  std::tie(ret, flag) = a.overflowing_div(c);
  EXPECT_FALSE(flag);
  EXPECT_EQ(ret, a / c);

  std::tie(ret, flag) = c.overflowing_div(b);
  EXPECT_FALSE(flag);
  EXPECT_EQ(ret, c / b);

  numbers::u128 zero = 0;
  EXPECT_DEATH(a.overflowing_div(zero), "");
}

TEST(u128OverflowTest, WrappingDiv) {
  numbers::u128 a = numbers::make_int128(21, 0);
  numbers::u128 b = 56;
  numbers::u128 c = 789;

  auto [ret, flag] = a.overflowing_div(b);
  EXPECT_FALSE(flag);
  EXPECT_EQ(ret, a.wrapping_div(b));

  std::tie(ret, flag) = b.overflowing_div(a);
  EXPECT_FALSE(flag);
  EXPECT_EQ(ret, b.wrapping_div(a));

  std::tie(ret, flag) = a.overflowing_div(c);
  EXPECT_FALSE(flag);
  EXPECT_EQ(ret, a.wrapping_div(c));

  std::tie(ret, flag) = c.overflowing_div(b);
  EXPECT_FALSE(flag);
  EXPECT_EQ(ret, c.wrapping_div(b));

  numbers::u128 zero = 0;
  EXPECT_DEATH(a.wrapping_div(zero), "");
}

TEST(u128OverflowTest, NegThrow) {
  numbers::u128 min = numbers::u128::MIN;
  numbers::u128 max = numbers::u128::MAX;
  EXPECT_THROW(-max, std::runtime_error);
  EXPECT_NO_THROW(-min);
  numbers::u128 a = 4578;
  EXPECT_THROW(-a, std::runtime_error);
}

TEST(u128OverflowTest, CheckedNeg) {
  numbers::u128 min = numbers::u128::MIN;
  numbers::u128 max = numbers::u128::MAX;
  EXPECT_EQ(max.checked_neg(), std::nullopt);
  EXPECT_EQ(min.checked_neg(), min);

  numbers::u128 a = 94578;
  EXPECT_EQ(a.checked_neg(), std::nullopt);
}

TEST(u128OverflowTest, OverflowingAndWrappingNeg) {
  numbers::u128 min = numbers::u128::MIN;
  numbers::u128 max = numbers::u128::MAX;
  auto [ret, flag] = max.overflowing_neg();
  EXPECT_TRUE(flag);
  EXPECT_EQ(ret, max.wrapping_neg());

  std::tie(ret, flag) = min.overflowing_neg();
  EXPECT_FALSE(flag);
  EXPECT_EQ(ret, min);

  numbers::u128 a = 93458;
  std::tie(ret, flag) = a.overflowing_neg();
  EXPECT_TRUE(flag);
  EXPECT_EQ(ret, a.wrapping_neg());
}
