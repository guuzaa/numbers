#include "gtest/gtest.h"

#include "integer.hh"

TEST(i128OverflowTest, AddThrow) {
  numbers::i128 max = numbers::i128::MAX;
  EXPECT_THROW(max + 1, std::runtime_error);
  EXPECT_THROW(1 + max, std::runtime_error);
  numbers::i128 min = numbers::i128::MIN;
  EXPECT_NO_THROW(min + max);
  EXPECT_THROW(-10 + min, std::runtime_error);

  EXPECT_THROW(max++, std::runtime_error);
  EXPECT_THROW(++max, std::runtime_error);
  EXPECT_THROW(min += -10, std::runtime_error);

  numbers::i128 a = 123;
  numbers::i128 b = 234;
  EXPECT_NO_THROW(a + b);
}

TEST(i128OverflowTest, CheckedAdd) {
  numbers::i128 max = numbers::i128::MAX;
  auto ret = max.checked_add(10);
  EXPECT_EQ(ret, std::nullopt);

  ret = max.checked_add(0);
  EXPECT_EQ(ret.value(), max);

  numbers::i128 min = numbers::i128::MIN;
  ret = min.checked_add(max);
  EXPECT_EQ(ret.value(), -1);

  ret = min.checked_add(-10);
  EXPECT_EQ(ret, std::nullopt);
}

TEST(i128OverflowTest, SaturatingAdd) {
  numbers::i128 max = numbers::i128::MAX;
  auto ret = max.saturating_add(10);
  EXPECT_EQ(ret, max);

  ret = (max - 1).saturating_add(1);
  EXPECT_EQ(ret, max);

  numbers::i128 min = numbers::i128::MIN;
  ret = min.saturating_add(-100);
  EXPECT_EQ(ret, min);

  numbers::i128 a = 456;
  numbers::i128 b = 789;
  ret = a.saturating_add(b);
  EXPECT_EQ(ret, 1245);
}

TEST(i128OverflowTest, OverflowingAdd) {
  numbers::i128 max = numbers::i128::MAX;
  numbers::i128 min = numbers::i128::MIN;
  auto [ret, flag] = max.overflowing_add(10);
  EXPECT_TRUE(flag);
  EXPECT_EQ(min + 9, ret);

  std::tie(ret, flag) = min.overflowing_add(-10);
  EXPECT_TRUE(flag);
  EXPECT_EQ(max - 9, ret);

  numbers::i128 a = 678;
  numbers::i128 b = 123;
  std::tie(ret, flag) = a.overflowing_add(b);
  EXPECT_FALSE(flag);
  EXPECT_EQ(801, ret);
}

TEST(i128OverflowTest, WrappingAdd) {
  numbers::i128 max = numbers::i128::MAX;
  numbers::i128 min = numbers::i128::MIN;

  auto ret = max.wrapping_add(10);
  EXPECT_EQ(min + 9, ret);

  ret = min.wrapping_add(-10);
  EXPECT_EQ(max - 9, ret);

  numbers::i128 a = 678;
  numbers::i128 b = 1234;
  ret = a.wrapping_add(b);
  EXPECT_EQ(1912, ret);
}

TEST(i128OverflowTest, SubThrow) {
  numbers::i128 min = numbers::i128::MIN;
  numbers::i128 max = numbers::i128::MAX;

  for (int n = 1; n < 1000; n++) {
    numbers::i128 num = n;
    EXPECT_THROW(min - num, std::runtime_error);
  }

  for (int n = -2; n > -1000; n--) {
    numbers::i128 num = n;
    EXPECT_THROW(num - max, std::runtime_error);
  }

  EXPECT_THROW(--min, std::runtime_error);
  EXPECT_THROW(min--, std::runtime_error);
  EXPECT_THROW(min -= 34, std::runtime_error);

  EXPECT_EQ(max - max, 0);
  EXPECT_EQ(min - min, 0);
  EXPECT_EQ(max -= max, 0);
  EXPECT_EQ(min -= min, 0);

  numbers::i128 a = 123;
  EXPECT_EQ(123 - a, 0);
}

TEST(i128OverflowTest, CheckedSub) {
  numbers::i128 min = numbers::i128::MIN;
  numbers::i128 max = numbers::i128::MAX;

  EXPECT_EQ(max.checked_sub(max), 0);
  EXPECT_EQ(min.checked_sub(min), 0);

  for (int n = 1; n < 1000; n++) {
    numbers::i128 num = n;
    auto ret = min.checked_sub(num);
    EXPECT_EQ(ret, std::nullopt);
  }

  for (int n = -2; n > -1000; n--) {
    numbers::i128 num = n;
    EXPECT_EQ(num.checked_sub(max), std::nullopt);
  }

  numbers::i128 a = 123;
  numbers::i128 b = 234;
  numbers::i128 c = 100;
  auto ret = b.checked_sub(a);
  EXPECT_EQ(ret, b - a);
  ret = c.checked_sub(a);
  EXPECT_EQ(ret, c - a);
}

TEST(i128OverflowTest, SaturatingSub) {
  numbers::i128 min = numbers::i128::MIN;
  numbers::i128 max = numbers::i128::MAX;
  for (int n = 1; n < 1000; n++) {
    numbers::i128 num = n;
    auto ret = min.saturating_sub(num);
    EXPECT_EQ(ret, min);
  }

  for (int n = -2; n > -1000; n--) {
    numbers::i128 num = n;
    EXPECT_EQ(num.saturating_sub(max), min);
  }

  numbers::i128 a = 123;
  numbers::i128 b = 234;
  numbers::i128 c = 100;
  auto ret = b.saturating_sub(a);
  EXPECT_EQ(ret, b - a);
  ret = c.saturating_sub(a);
  EXPECT_EQ(ret, c - a);
}

TEST(i128OverflowTest, OverflowingSub) {
  numbers::i128 min = numbers::i128::MIN;
  numbers::i128 max = numbers::i128::MAX;

  for (int n = 1; n < 1000; n++) {
    numbers::i128 num = n;
    auto [ret, flag] = min.overflowing_sub(num);
    EXPECT_TRUE(flag);
    EXPECT_EQ(ret, max - num + 1);
  }

  for (int n = -2; n > -1000; n--) {
    numbers::i128 num = n;
    auto [ret, flag] = num.overflowing_sub(max);
    EXPECT_TRUE(flag);
    EXPECT_EQ(ret, max + num + 2);
  }

  numbers::i128 a = 123;
  numbers::i128 b = 567;
  numbers::i128 c = 100;
  auto [ret, flag] = b.overflowing_sub(a);
  EXPECT_FALSE(flag);
  EXPECT_EQ(ret, b - a);
  std::tie(ret, flag) = c.overflowing_sub(a);
  EXPECT_FALSE(flag);
  EXPECT_EQ(ret, c - a);
}

TEST(i128OverflowTest, WrappingSub) {
  numbers::i128 min = numbers::i128::MIN;
  numbers::i128 max = numbers::i128::MAX;

  for (int n = 1; n < 1000; n++) {
    numbers::i128 num = n;
    auto ret = min.wrapping_sub(num);
    EXPECT_EQ(ret, max - num + 1);
  }

  for (int n = -2; n > -1000; n--) {
    numbers::i128 num = n;
    auto ret = num.wrapping_sub(max);
    EXPECT_EQ(ret, max + num + 2);
  }

  numbers::i128 a = 1234;
  numbers::i128 b = 5678;
  numbers::i128 c = 100;
  auto ret = b.wrapping_sub(a);
  EXPECT_EQ(ret, b - a);
  ret = c.wrapping_sub(a);
  EXPECT_EQ(ret, c - a);
}

TEST(i128OverflowTest, MulThrow) {
  numbers::i128 min = numbers::i128::MIN;
  numbers::i128 max = numbers::i128::MAX;
  EXPECT_THROW(max * 12, std::runtime_error);
  EXPECT_THROW(min * 345, std::runtime_error);
}

TEST(i128OverflowTest, CheckedMul) {
  numbers::i128 min = numbers::i128::MIN;
  numbers::i128 max = numbers::i128::MAX;
  auto ret = max.checked_mul(123);
  EXPECT_EQ(ret, std::nullopt);
  ret = min.checked_mul(34);
  EXPECT_EQ(ret, std::nullopt);
}

TEST(i128OverflowTest, SaturatingMul) {
  numbers::i128 min = numbers::i128::MIN;
  numbers::i128 max = numbers::i128::MAX;
  auto ret = max.saturating_mul(123);
  EXPECT_EQ(ret, max);
  ret = min.saturating_mul(34);
  EXPECT_EQ(ret, min);
}

TEST(i128OverflowTest, OverflowingMul) {
  numbers::i128 min = numbers::i128::MIN;
  numbers::i128 max = numbers::i128::MAX;
  auto [_, flag] = max.overflowing_mul(123);
  EXPECT_TRUE(flag);

  std::tie(_, flag) = min.overflowing_mul(34);
  EXPECT_TRUE(flag);
}

TEST(i128OverflowTest, WrappingMul) {
  numbers::i128 min = numbers::i128::MIN;
  numbers::i128 max = numbers::i128::MAX;
  numbers::i128 a = 345;
  numbers::i128 b = 12;
  numbers::i128 c = -678;

  auto [ret, flag] = max.overflowing_mul(a);
  EXPECT_TRUE(flag);
  EXPECT_EQ(ret, max.wrapping_mul(a));

  std::tie(ret, flag) = min.overflowing_mul(b);
  EXPECT_TRUE(flag);
  EXPECT_EQ(ret, min.wrapping_mul(b));

  EXPECT_EQ(a.wrapping_mul(b), a * b);  // +, +
  EXPECT_EQ(b.wrapping_mul(a), b * a);
  EXPECT_EQ(b.wrapping_mul(c), b * c);  // +, -
  EXPECT_EQ(c.wrapping_mul(a), a * c);  // -, +
  EXPECT_EQ(c.wrapping_mul(c), c * c);  // -, -
}

TEST(i128OverflowTest, DivThrow) {
  numbers::i128 min = numbers::i128::MIN;
  EXPECT_THROW(min / -1, std::runtime_error);

  numbers::i128 a = 123;
  numbers::i128 b = 456;
  numbers::i128 c = -789;

  EXPECT_NO_THROW(a / b);  // +/+
  EXPECT_NO_THROW(b / a);

  EXPECT_NO_THROW(a / c);  // +/-
  EXPECT_NO_THROW(c / a);  // -/+

  b = -b;
  EXPECT_NO_THROW(c / b);  // -/-
  EXPECT_NO_THROW(b / c);  // -/-

  numbers::i128 zero = 0;
  EXPECT_DEATH(a / zero, "Assertion failed");
}

TEST(i128OverflowTest, CheckedDiv) {
  numbers::i128 min = numbers::i128::MIN;
  auto ret = min.checked_div(-1);
  EXPECT_EQ(ret, std::nullopt);

  numbers::i128 a = 321;
  numbers::i128 b = 564;
  numbers::i128 c = -798;
  EXPECT_EQ(a.checked_div(b).value(), a / b);
  EXPECT_EQ(b.checked_div(a).value(), b / a);

  EXPECT_EQ(a.checked_div(c).value(), a / c);
  EXPECT_EQ(c.checked_div(b).value(), c / b);

  a = -a;
  EXPECT_EQ(a.checked_div(c).value(), a / c);
  EXPECT_EQ(c.checked_div(a).value(), c / a);
}

TEST(i128OverflowTest, SaturatingDiv) {
  numbers::i128 min = numbers::i128::MIN;
  auto ret = min.saturating_div(-1);
  EXPECT_EQ(ret, min);

  numbers::i128 a = 21;
  numbers::i128 b = 56;
  numbers::i128 c = -78;
  EXPECT_EQ(a.saturating_div(b), a / b);
  EXPECT_EQ(b.saturating_div(a), b / a);

  EXPECT_EQ(a.saturating_div(c), a / c);
  EXPECT_EQ(c.saturating_div(b), c / b);

  a = -a;
  EXPECT_EQ(a.saturating_div(c), a / c);
  EXPECT_EQ(c.saturating_div(a), c / a);
}

TEST(i128OverflowTest, OverflowingDiv) {
  numbers::i128 min = numbers::i128::MIN;
  auto [ret, flag] = min.overflowing_div(-1);
  EXPECT_TRUE(flag);

  numbers::i128 a = 21;
  numbers::i128 b = 56;
  numbers::i128 c = -78;

  std::tie(ret, flag) = a.overflowing_div(b);
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

  a = -a;
  std::tie(ret, flag) = a.overflowing_div(c);
  EXPECT_FALSE(flag);
  EXPECT_EQ(ret, a / c);

  std::tie(ret, flag) = c.overflowing_div(a);
  EXPECT_FALSE(flag);
  EXPECT_EQ(ret, c / a);
}

TEST(i128OverflowTest, WrappingDiv) {
  numbers::i128 min = numbers::i128::MIN;
  auto [ret, flag] = min.overflowing_div(-1);
  EXPECT_TRUE(flag);
  EXPECT_EQ(ret, min.wrapping_div(-1));

  numbers::i128 a = numbers::make_int128(21, 0);
  numbers::i128 b = 56;
  numbers::i128 c = -78;

  std::tie(ret, flag) = a.overflowing_div(b);
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

  a = -a;
  std::tie(ret, flag) = a.overflowing_div(c);
  EXPECT_FALSE(flag);
  EXPECT_EQ(ret, a.wrapping_div(c));

  std::tie(ret, flag) = c.overflowing_div(a);
  EXPECT_FALSE(flag);
  EXPECT_EQ(ret, c.wrapping_div(a));
}

TEST(i128OverflowTest, AbsThrow) {
  numbers::i128 min = numbers::i128::MIN;
  numbers::i128 max = numbers::i128::MAX;
  numbers::i128 a = -3456;
  EXPECT_THROW(min.abs(), std::runtime_error);
  EXPECT_EQ(a.abs(), -a);
  EXPECT_NO_THROW(max.abs());
}

TEST(i128OverflowTest, CheckedAbs) {
  numbers::i128 min = numbers::i128::MIN;
  numbers::i128 max = numbers::i128::MAX;
  numbers::i128 a = -34567;
  EXPECT_EQ(min.checked_abs(), std::nullopt);
  EXPECT_EQ(a.checked_abs().value(), -a);
  EXPECT_EQ(max.checked_abs().value(), max);
}

TEST(i128OverflowTest, SaturatingAbs) {
  numbers::i128 min = numbers::i128::MIN;
  numbers::i128 max = numbers::i128::MAX;
  EXPECT_EQ(min.saturating_abs(), max);
  EXPECT_EQ(max.saturating_abs(), max);
  numbers::i128 a = -345678;
  EXPECT_EQ(a.saturating_abs(), -a);
}

TEST(i128OverflowTest, OverflowingAndWrappingAbs) {
  numbers::i128 min = numbers::i128::MIN;
  numbers::i128 max = numbers::i128::MAX;
  auto [ret, flag] = min.overflowing_abs();
  EXPECT_TRUE(flag);
  EXPECT_EQ(ret, min);
  EXPECT_EQ(ret, min.wrapping_abs());

  std::tie(ret, flag) = max.overflowing_abs();
  EXPECT_FALSE(flag);
  EXPECT_EQ(ret, max);
  EXPECT_EQ(ret, max.wrapping_abs());

  numbers::i128 a = -45678;
  std::tie(ret, flag) = a.overflowing_abs();
  EXPECT_FALSE(flag);
  EXPECT_EQ(ret, -a);
  EXPECT_EQ(ret, a.wrapping_abs());
}

TEST(i128OverflowTest, NegThrow) {
  numbers::i128 min = numbers::i128::MIN;
  numbers::i128 max = numbers::i128::MAX;
  EXPECT_THROW(-min, std::runtime_error);
  EXPECT_NO_THROW(-max);
  numbers::i128 a = -4578;
  EXPECT_NO_THROW(-a);
}

TEST(i128OverflowTest, CheckedNeg) {
  numbers::i128 min = numbers::i128::MIN;
  numbers::i128 max = numbers::i128::MAX;
  EXPECT_EQ(min.checked_neg(), std::nullopt);
  EXPECT_EQ(max.checked_neg(), -max);
  numbers::i128 a = -94578;
  EXPECT_EQ(a.checked_neg().value(), -a);
}

TEST(i128OverflowTest, SaturatingNeg) {
  numbers::i128 min = numbers::i128::MIN;
  numbers::i128 max = numbers::i128::MAX;
  EXPECT_EQ(min.saturating_neg(), max);
  EXPECT_EQ(max.saturating_neg(), -max);
  numbers::i128 a = -9458;
  EXPECT_EQ(a.saturating_neg(), -a);
}

TEST(i128OverflowTest, OverflowingAndWrappingNeg) {
  numbers::i128 min = numbers::i128::MIN;
  numbers::i128 max = numbers::i128::MAX;
  auto [ret, flag] = min.overflowing_neg();
  EXPECT_TRUE(flag);
  EXPECT_EQ(ret, min);
  EXPECT_EQ(ret, min.wrapping_neg());

  std::tie(ret, flag) = max.overflowing_neg();
  EXPECT_EQ(ret, -max);
  EXPECT_EQ(ret, max.wrapping_neg());

  numbers::i128 a = -93458;
  std::tie(ret, flag) = a.overflowing_neg();
  EXPECT_EQ(ret, -a);
  EXPECT_EQ(ret, a.wrapping_neg());
}
