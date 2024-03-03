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