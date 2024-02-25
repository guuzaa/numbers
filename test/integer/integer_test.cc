#include <numeric>
#include <vector>
#include "gtest/gtest.h"

#include "integer.hh"
#include "test/utils.hh"

using namespace numbers;

TEST(IntegerTest, IntegerMinMax) {
  ASSERT_THROW(-int64::MIN, std::runtime_error);
  ASSERT_EQ(int16::MIN.saturating_neg(), int16::MAX);
  ASSERT_EQ(int16::MAX.saturating_add(int16::MAX), int16::MAX);
  ASSERT_EQ(int16::MAX.saturating_sub(int16::MAX), 0);
}

TEST(integerTest, integerStringStream) {
  for (short n = 1; n < 1000; n++) {
    int16 num = n;
    std::ostringstream stream;
    stream << num;
    ASSERT_EQ(stream.str(), std::to_string(n));
  }

  for (long long n = 1; n < 1000; n++) {
    int64 num = n;
    std::ostringstream stream;
    stream << num;
    ASSERT_EQ(stream.str(), std::to_string(n));
  }
}

TEST(integerTest, integerAdd) {
  int32 num = 10;
  int32 num1 = 10;
  ASSERT_EQ(num, num1);

  int32 num2 = 20;
  int32 zero_num = 0;
  int32 max_num = int32::MAX;
  int32 min_num = int32::MIN;

  ASSERT_EQ(num + num1, num2);
  ASSERT_EQ(zero_num + max_num, max_num);
  ASSERT_EQ(max_num + min_num, -1);
}

TEST(integerTest, integerAddNoSideEffects) {
  int32 num = 10;
  int32 num1 = num;
  int32 num2 = num;
  ASSERT_EQ(num1, num);
  ASSERT_EQ(num2, num);

  ASSERT_EQ(num2 + num1, 20);
  ASSERT_EQ(num1, num);
  ASSERT_EQ(num2, num);
}

TEST(integerTest, integerWrappingAdd) {
  for (int16_t n = std::numeric_limits<int16_t>::min(); n < std::numeric_limits<int16_t>::max(); n++) {
    int16 num = n;
    ASSERT_EQ(num.wrapping_add(1), int16(n + 1));
  }
  int16 num = int16::MAX;
  ASSERT_EQ(num.wrapping_add(1), int16::MIN);

  constexpr int32_t min = 10;
  constexpr int32_t max = 90000;
  for (int run = 0; run < 100; run++) {
    int32_t a = random_integer(min, max);
    int32_t b = random_integer(min, max);
    int32 num_a = a;
    int32 num_b = b;
    ASSERT_EQ(int32(a + b), num_a + num_b);
    ASSERT_EQ(num_a + num_b, a + b);
  }
}

TEST(integerTest, integerWrappingAddNoSideEffects) {
  int32 num = 10;
  int32 num1 = num;
  int32 num2 = num;
  ASSERT_EQ(num1, num);
  ASSERT_EQ(num2, num);

  ASSERT_EQ(num2.wrapping_add(num1), int32(20));
  ASSERT_EQ(num1, num);
  ASSERT_EQ(num2, num);
}

TEST(integerTest, integerAddByAccumulate) {
  constexpr size_t len = 100;
  std::vector<int64> nums(len);
  ASSERT_EQ(nums.size(), len);
  std::iota(nums.begin(), nums.end(), int64(1));
  ASSERT_EQ(std::accumulate(nums.begin(), nums.end(), int64(0)), int64(5050));
}

TEST(integerTest, integerCheckedAdd) {
  constexpr int32_t min = 10;
  constexpr int32_t max = 90000;
  for (int run = 0; run < 100; run++) {
    int32_t a = random_integer(min, max);
    int32_t b = random_integer(min, max);
    int32 num_a = a;
    int32 num_b = b;
    ASSERT_EQ(num_a.checked_add(num_b).value(), a + b);
  }

  {
    int16 num = 10;
    int16 num1 = int16::MAX;
    ASSERT_EQ(num.checked_add(num1), std::nullopt);
  }

  {
    int64 num = 10;
    int64 num1 = int64::MAX;
    ASSERT_EQ(num.checked_add(num1), std::nullopt);
  }

  {
    int64 num = -1;
    int64 num1 = int64::MIN;
    ASSERT_EQ(num.checked_add(num1), std::nullopt);
  }

  {
    int64 num1 = int64::MIN;
    ASSERT_EQ(num1.checked_add(num1), std::nullopt);
    int64 num2 = int64::MAX;
    ASSERT_EQ(num2.checked_add(num2), std::nullopt);
  }
}

TEST(integerTest, integerCheckedAddNoSideEffects) {
  int32 num = 10;
  int32 num1 = num;
  int32 num2 = num;
  ASSERT_EQ(num1, num);
  ASSERT_EQ(num2, num);

  ASSERT_EQ(num2.checked_add(num1).value(), int32(20));
  ASSERT_EQ(num1, num);
  ASSERT_EQ(num2, num);
}

TEST(integerTest, integerOverflowingAdd) {
  constexpr int32_t min = 10;
  constexpr int32_t max = 90000;
  for (int run = 0; run < 100; run++) {
    int32_t a = random_integer(min, max);
    int32_t b = random_integer(min, max);
    int32 num_a = a;
    int32 num_b = b;
    auto [ret, flag] = num_a.overflowing_add(num_b);
    ASSERT_FALSE(flag);
    ASSERT_EQ(ret, a + b);
  }

  {
    int16 num = 10;
    int16 num1 = int16::MAX;
    auto [ret, flag] = num.overflowing_add(num1);
    ASSERT_TRUE(flag);
    ASSERT_EQ(ret, 10 + std::numeric_limits<int16_t>::max());
  }

  {
    int64 num = 10;
    int64 num1 = int64::MAX;
    auto [_, flag] = num.overflowing_add(num1);
    ASSERT_TRUE(flag);
  }

  {
    int64 num = -1;
    int64 num1 = int64::MIN;
    auto [_, flag] = num.overflowing_add(num1);
    ASSERT_TRUE(flag);
  }

  {
    int64 num1 = int64::MIN;
    auto [_, flag1] = num1.overflowing_add(num1);
    ASSERT_TRUE(flag1);
  }

  {
    int64 num2 = int64::MAX;
    auto [_, flag2] = num2.overflowing_add(num2);
    ASSERT_TRUE(flag2);
  }
}

TEST(integerTest, integerOverflowingAddNoSideEffects) {
  int32 num = 10;
  int32 num1 = num;
  int32 num2 = num;
  ASSERT_EQ(num1, num);
  ASSERT_EQ(num2, num);

  auto [ret, flag] = num2.overflowing_add(num1);
  ASSERT_EQ(ret, int32(20));
  ASSERT_FALSE(flag);
  ASSERT_EQ(num1, num);
  ASSERT_EQ(num2, num);
}

TEST(integerTest, integerSaturatingAdd) {
  constexpr int32_t min = 10;
  constexpr int32_t max = 90000;
  for (int run = 0; run < 100; run++) {
    int32_t a = random_integer(min, max);
    int32_t b = random_integer(min, max);
    int32 num_a = a;
    int32 num_b = b;
    auto ret = num_a.saturating_add(num_b);
    ASSERT_EQ(ret, a + b);
  }

  for (int8_t i = 1; i < std::numeric_limits<int8_t>::max(); i++) {
    int8 num = int8::MAX;
    auto ret = num.saturating_add(int8(i));
    EXPECT_EQ(ret, int8::MAX) << static_cast<int>(i) << '\n';
  }

  {
    int16 num = 10;
    int16 num1 = int16::MAX;
    auto ret = num.saturating_add(num1);
    ASSERT_EQ(ret, int16::MAX);
  }

  {
    int64 num = 10;
    int64 num1 = int64::MAX;
    auto ret = num.saturating_add(num1);
    ASSERT_EQ(ret, int64::MAX);
  }

  {
    int64 max_num = int64::MAX;
    auto ret = max_num.saturating_add(max_num);
    ASSERT_EQ(ret, int64::MAX);

    int64 min_num = int64::MIN;
    ret = min_num.saturating_add(min_num);
    ASSERT_EQ(ret, int64::MIN);
  }

  {
    int64 num = -1;
    int64 num1 = int64::MIN;
    auto ret = num.saturating_add(num1);
    ASSERT_EQ(ret, int64::MIN);
  }
}

TEST(integerTest, integerSaturatingAddNoSideEffects) {
  int32 num = 10;
  int32 num1 = num;
  int32 num2 = num;
  ASSERT_EQ(num1, num);
  ASSERT_EQ(num2, num);

  auto ret = num2.saturating_add(num1);
  ASSERT_EQ(ret, int32(20));
  ASSERT_EQ(num1, num);
  ASSERT_EQ(num2, num);
}

TEST(integerTest, integerAddOverflow) {
  {
    int16 num = 10;
    int16 num1 = int16::MAX;
    ASSERT_THROW(num + num1, std::runtime_error);
  }

  {
    int64 num = 10;
    int64 num1 = int64::MAX;
    ASSERT_THROW(num + num1, std::runtime_error);
  }

  {
    int64 num = -1;
    int64 num1 = int64::MIN;
    ASSERT_THROW(num + num1, std::runtime_error);
  }
}

TEST(integerTest, integerAddOverflowByAccumulate) {
  std::vector<int64> nums(10);
  ASSERT_EQ(nums.size(), 10);
  std::iota(nums.begin(), nums.end(), int64::MAX - 11);
  ASSERT_THROW(std::accumulate(nums.begin(), nums.end(), int64(0)), std::runtime_error);
}

TEST(integerTest, integerSub) {
  auto num = int32(10);
  auto num1 = int32(10);
  auto num2 = int32(20);
  int32 num3 = int32::MAX;
  auto num4 = int32(0);

  ASSERT_EQ(num2 - num1, num);
  ASSERT_EQ(num - num1, num4);
  ASSERT_EQ(num3 - num4, num3);

  {
    int64 num = -1;
    int64 num1 = int64::MIN;
    ASSERT_EQ(num - num1, int64::MAX);
  }

  constexpr int32_t min = 10;
  constexpr int32_t max = 90000;

  for (int run = 0; run < 100; run++) {
    int32_t a = random_integer(min, max);
    int32_t b = random_integer(min, max);
    int32 num_a = a;
    int32 num_b = b;
    ASSERT_EQ(int32(a - b), num_a - num_b);
    ASSERT_EQ(num_a - num_b, a - b);
  }
}

TEST(integerTest, integerSubNoSideEffects) {
  int32 num = 10;
  int32 num1 = num;
  int32 num2 = num;
  ASSERT_EQ(num1, num);
  ASSERT_EQ(num2, num);

  ASSERT_EQ(num2 - num1, int32(0));
  ASSERT_EQ(num1, num);
  ASSERT_EQ(num2, num);
}

TEST(integerTest, integerSubOverflow) {
  for (short n = 1; n < 1000; n++) {
    int16 num = n;
    int16 num1 = int16::MIN;
    ASSERT_THROW(num1 - num, std::runtime_error);
  }
}

TEST(integerTest, integerCheckedSub) {
  for (short n = 1; n < 1000; n++) {
    int16 num = n;
    int16 num1 = int16::MIN;
    ASSERT_EQ(num1.checked_sub(num), std::nullopt);
  }

  for (int16_t n = std::numeric_limits<int16_t>::max(); n > std::numeric_limits<int16_t>::min(); n--) {
    int16 num = n;
    auto ret = num.checked_sub(1);
    ASSERT_EQ(ret.value(), int16(n - 1));
  }
}

TEST(integerTest, integerCheckedSubNoSideEffects) {
  int32 num = 10;
  int32 num1 = num;
  int32 num2 = num;
  ASSERT_EQ(num1, num);
  ASSERT_EQ(num2, num);

  ASSERT_EQ(num2.checked_sub(num1).value(), int32(0));
  ASSERT_EQ(num1, num);
  ASSERT_EQ(num2, num);
}

TEST(integerTest, integerOverflowingSub) {
  for (short n = 1; n < 1000; n++) {
    int16 num = n;
    int16 num1 = int16::MIN;
    auto [ret, flag] = num1.overflowing_sub(num);
    ASSERT_TRUE(flag);
    ASSERT_EQ(ret, std::numeric_limits<int16_t>::min() - n);
  }

  for (int16_t n = std::numeric_limits<int16_t>::max(); n > std::numeric_limits<int16_t>::min(); n--) {
    int16 num = n;
    auto [ret, flag] = num.overflowing_sub(1);
    ASSERT_FALSE(flag);
    ASSERT_EQ(ret, int16(n - 1));
  }
}

TEST(integerTest, integerOverflowingSubNoSideEffects) {
  int32 num = 10;
  int32 num1 = num;
  int32 num2 = num;
  ASSERT_EQ(num1, num);
  ASSERT_EQ(num2, num);

  auto [ret, flag] = num2.overflowing_sub(num1);
  ASSERT_EQ(ret, int32(0));
  ASSERT_FALSE(flag);
  ASSERT_EQ(num1, num);
  ASSERT_EQ(num2, num);
}

TEST(integerTest, integerWrappingSub) {
  for (int16_t n = std::numeric_limits<int16_t>::max(); n > std::numeric_limits<int16_t>::min(); n--) {
    int16 num = n;
    ASSERT_EQ(num.wrapping_sub(1), int16(n - 1));
  }
  int16 num = int16::MIN;
  ASSERT_EQ(num.wrapping_sub(1), int16::MAX);
}

TEST(integerTest, integerWarppingSubNoSideEffects) {
  int32 num = 10;
  int32 num1 = num;
  int32 num2 = num;
  ASSERT_EQ(num1, num);
  ASSERT_EQ(num2, num);

  auto ret = num2.wrapping_sub(num1);
  ASSERT_EQ(ret, int32(0));
  ASSERT_EQ(num1, num);
  ASSERT_EQ(num2, num);
}

TEST(integerTest, integerSaturatingSub) {
  for (short n = 1; n <= 1000; n++) {
    int16 num = n;
    int16 num1 = int16::MIN;
    auto ret = num1.saturating_sub(num);
    ASSERT_EQ(ret, int16::MIN);
  }

  for (int16_t n = std::numeric_limits<int16_t>::max(); n > std::numeric_limits<int16_t>::min(); n--) {
    int16 num = n;
    auto ret = num.saturating_sub(1);
    ASSERT_EQ(ret, int16(n - 1));
  }

  int16 min_num = int16::MIN;
  auto ret = min_num.saturating_sub(1);
  ASSERT_EQ(ret, int16::MIN);

  int16 max_num = int16::MAX;
  ret = max_num.saturating_sub(min_num);
  ASSERT_EQ(ret, int16::MAX);

  ret = min_num.saturating_sub(max_num);
  ASSERT_EQ(ret, int16::MIN);
}

TEST(integerTest, integerSaturatingSubNoSideEffects) {
  int32 num = 10;
  int32 num1 = num;
  int32 num2 = num;
  ASSERT_EQ(num1, num);
  ASSERT_EQ(num2, num);

  auto ret = num2.saturating_sub(num1);
  ASSERT_EQ(ret, int32(0));
  ASSERT_EQ(num1, num);
  ASSERT_EQ(num2, num);
}

TEST(integerTest, integerMul) {
  auto num = int64(10);
  auto num1 = int64(100);
  ASSERT_EQ(num1 * num, 1000);
  num = num.MAX;
  num1 = int64(0);
  ASSERT_EQ(num1 * num, 0);
  num1 = int64(1);
  ASSERT_EQ(num1 * num, num);

  constexpr int32_t min = 10;
  constexpr int32_t max = 10000;
  for (int run = 0; run < 100; run++) {
    int32_t a = random_integer(min, max);
    int32_t b = random_integer(min, max);
    int32 num_a = a;
    int32 num_b = b;
    ASSERT_EQ(int32(a * b), num_a * num_b);
    ASSERT_EQ(num_a * num_b, a * b);
  }
}

TEST(integerTest, integerMulNoSideEffects) {
  int32 num = 10;
  int32 num1 = num;
  int32 num2 = num;
  ASSERT_EQ(num1, num);
  ASSERT_EQ(num2, num);

  auto ret = num2 * num1;
  ASSERT_EQ(ret, int32(100));
  ASSERT_EQ(num1, num);
  ASSERT_EQ(num2, num);
}

TEST(integerTest, integerMulOverflow) {
  auto num = int8(10);
  auto num1 = int8(100);
  ASSERT_THROW(num1 * num, std::runtime_error);
}

TEST(integerTest, integerCheckedMul) {
  auto num = int8(10);
  auto num1 = int8(100);
  ASSERT_EQ(num1.checked_mul(num), std::nullopt);

  constexpr int32_t min = 10;
  constexpr int32_t max = 10000;
  for (int run = 0; run < 100; run++) {
    int32_t a = random_integer(min, max);
    int32_t b = random_integer(min, max);
    int32 num_a = a;
    int32 num_b = b;
    ASSERT_EQ(num_a.checked_mul(num_b).value(), a * b);
  }
}

TEST(integerTest, integerCheckedMulNoSideEffects) {
  int32 num = 10;
  int32 num1 = num;
  int32 num2 = num;
  ASSERT_EQ(num1, num);
  ASSERT_EQ(num2, num);

  auto ret = num2.checked_mul(num1);
  ASSERT_EQ(ret.value(), int32(100));
  ASSERT_EQ(num1, num);
  ASSERT_EQ(num2, num);
}

TEST(integerTest, integerOverflowingMul) {
  auto num = int8(10);
  auto num1 = int8(100);
  auto [ret, flag] = num1.overflowing_mul(num);
  ASSERT_EQ(ret, int8_t(10 * 100));
  ASSERT_TRUE(flag);

  constexpr int32_t min = 10;
  constexpr int32_t max = 10000;
  for (int run = 0; run < 100; run++) {
    int32_t a = random_integer(min, max);
    int32_t b = random_integer(min, max);
    int32 num_a = a;
    int32 num_b = b;
    auto [ret, flag] = num_a.overflowing_mul(num_b);
    ASSERT_FALSE(flag);
    ASSERT_EQ(ret, a * b);
  }
}

TEST(integerTest, integerOverflowingMulNoSideEffects) {
  int32 num = 10;
  int32 num1 = num;
  int32 num2 = num;
  ASSERT_EQ(num1, num);
  ASSERT_EQ(num2, num);

  auto [ret, flag] = num2.overflowing_mul(num1);
  ASSERT_FALSE(flag);
  ASSERT_EQ(ret, int32(100));
  ASSERT_EQ(num1, num);
  ASSERT_EQ(num2, num);
}

TEST(integerTest, integerSaturatingMul) {
  auto num = int8(10);
  auto num1 = int8(100);
  auto ret = num1.saturating_mul(num);
  ASSERT_EQ(ret, int8::MAX);

  int16 max_num = int16::MAX;
  ASSERT_EQ(max_num.saturating_mul(max_num), int16::MAX);
  int16 min_num = int16::MIN;
  int16 num2 = int16{-30};
  ASSERT_EQ(min_num.saturating_mul(min_num), int16::MAX);
  ASSERT_EQ(min_num.saturating_mul(num2), int16::MAX);

  constexpr int32_t min = 10;
  constexpr int32_t max = 10000;
  for (int run = 0; run < 100; run++) {
    int32_t a = random_integer(min, max);
    int32_t b = random_integer(min, max);
    int32 num_a = a;
    int32 num_b = b;
    auto ret = num_a.saturating_mul(num_b);
    ASSERT_EQ(ret, a * b);
  }
}

TEST(integerTest, integerSaturatingMulNoSideEffects) {
  int32 num = 10;
  int32 num1 = num;
  int32 num2 = num;
  ASSERT_EQ(num1, num);
  ASSERT_EQ(num2, num);

  auto ret = num2.saturating_mul(num1);
  ASSERT_EQ(ret, int32(100));
  ASSERT_EQ(num1, num);
  ASSERT_EQ(num2, num);
}

TEST(integerTest, integerWrappingMul) {
  int16 min_num = int16::MIN;
  int16 max_num = int16::MAX;
  int16 num1 = -1;
  ASSERT_EQ(min_num.wrapping_mul(num1), min_num);
  ASSERT_EQ(max_num.wrapping_mul(num1), min_num + 1);
}

TEST(integerTest, integerWrappingMulNoSideEffects) {
  int32 num = 10;
  int32 num1 = num;
  int32 num2 = num;
  ASSERT_EQ(num1, num);
  ASSERT_EQ(num2, num);

  auto ret = num2.wrapping_mul(num1);
  ASSERT_EQ(ret, int32(100));
  ASSERT_EQ(num1, num);
  ASSERT_EQ(num2, num);
}

TEST(integerTest, integerDiv) {
  constexpr int32_t min = 10;
  constexpr int32_t max = 10000;
  for (int run = 0; run < 100; run++) {
    int32_t a = random_integer(min, max);
    int32_t b = random_integer(min, max);
    int32 num_a = a;
    int32 num_b = b;
    ASSERT_EQ(int32(a / b), num_a / num_b);
    EXPECT_EQ(num_a / num_b, a / b) << a << ' ' << b << '\n';
  }
}

TEST(integerTest, integerDivNoSideEffects) {
  int32 num = 10;
  int32 num1 = num;
  int32 num2 = num;
  ASSERT_EQ(num1, num);
  ASSERT_EQ(num2, num);

  auto ret = num2 / num1;
  ASSERT_EQ(ret, int32(1));
  ASSERT_EQ(num1, num);
  ASSERT_EQ(num2, num);
}

TEST(integerTest, integerDivOverflow) {
  int8 num = -1;
  int8 num1 = int8::MIN;
  ASSERT_THROW(num1 / num, std::runtime_error);
}

TEST(integerTest, integerWrappingDiv) {
  int8 num = -1;
  int8 num1 = int8::MIN;
  ASSERT_EQ(num1.wrapping_div(num), num1);
}

TEST(integerTest, integerWrappingDivNoSideEffects) {
  int32 num = 10;
  int32 num1 = num;
  int32 num2 = num;
  ASSERT_EQ(num1, num);
  ASSERT_EQ(num2, num);

  auto ret = num2.wrapping_div(num1);
  ASSERT_EQ(ret, int32(1));
  ASSERT_EQ(num1, num);
  ASSERT_EQ(num2, num);
}

TEST(integerTest, integerCheckedDiv) {
  constexpr int32_t min = 10;
  constexpr int32_t max = 10000;
  for (int run = 0; run < 100; run++) {
    int32_t a = random_integer(min, max);
    int32_t b = random_integer(min, max);
    int32 num_a = a;
    int32 num_b = b;
    EXPECT_EQ(num_a.checked_div(num_b).value(), a / b);
  }

  int8 num = -1;
  int8 num1 = int8::MIN;
  ASSERT_EQ(num1.checked_div(num), std::nullopt);
}

TEST(integerTest, integerCheckedDivNoSideEffects) {
  int32 num = 10;
  int32 num1 = num;
  int32 num2 = num;
  ASSERT_EQ(num1, num);
  ASSERT_EQ(num2, num);

  auto ret = num2.checked_div(num1);
  ASSERT_EQ(ret.value(), int32(1));
  ASSERT_EQ(num1, num);
  ASSERT_EQ(num2, num);
}

TEST(integerTest, integerOverflowingDiv) {
  constexpr int32_t min = 10;
  constexpr int32_t max = 10000;
  for (int run = 0; run < 100; run++) {
    int32_t a = random_integer(min, max);
    int32_t b = random_integer(min, max);
    int32 num_a = a;
    int32 num_b = b;
    auto [ret, flag] = num_a.overflowing_div(num_b);
    ASSERT_FALSE(flag);
    EXPECT_EQ(ret, a / b);
  }

  int8 num = -1;
  int8 num1 = int8::MIN;
  auto [ret, flag] = num1.overflowing_div(num);
  ASSERT_TRUE(flag);
  ASSERT_EQ(ret, -1 * std::numeric_limits<int8_t>::min());
}

TEST(integerTest, integerOverflowingDivNoSideEffects) {
  int32 num = 10;
  int32 num1 = num;
  int32 num2 = num;
  ASSERT_EQ(num1, num);
  ASSERT_EQ(num2, num);

  auto [ret, flag] = num2.overflowing_div(num1);
  ASSERT_FALSE(flag);
  ASSERT_EQ(num1, num);
  ASSERT_EQ(num2, num);
}

TEST(integerTest, integerSaturatingDiv) {
  constexpr int32_t min = 10;
  constexpr int32_t max = 10000;
  for (int run = 0; run < 100; run++) {
    int32_t a = random_integer(min, max);
    int32_t b = random_integer(min, max);
    int32 num_a = a;
    int32 num_b = b;
    auto ret = num_a.saturating_div(num_b);
    EXPECT_EQ(ret, a / b);
  }

  int8 num = -1;
  int8 num1 = int8::MIN;
  auto ret = num1.saturating_div(num);
  ASSERT_EQ(ret, int8::MIN);
}

TEST(integerTest, integerSaturatingDivNoSideEffects) {
  int32 num = 10;
  int32 num1 = num;
  int32 num2 = num;
  ASSERT_EQ(num1, num);
  ASSERT_EQ(num2, num);

  auto ret = num2.saturating_div(num1);
  ASSERT_EQ(num1, num);
  ASSERT_EQ(num2, num);
}

TEST(integerTest, integerAbs) {
  for (int32_t i = 0; i <= 1000; i += 10) {
    int32 num = i;
    ASSERT_EQ(num.abs(), num);
  }

  for (int32_t i = -1000; i <= 0; i += 10) {
    int32 num = i;
    ASSERT_EQ(num.abs(), int32(-i));
  }

  int32 max_num = int32::MAX;
  ASSERT_EQ(max_num.abs(), max_num);
  int32 min_num = int32::MIN;
  ASSERT_THROW(min_num.abs(), std::runtime_error);
}

TEST(integerTest, integerAbsNoSideEffects) {
  int32 num = 10;
  int32 tmp_num = num;
  ASSERT_EQ(num, tmp_num);

  auto _ = num.abs();
  ASSERT_EQ(num, tmp_num);
}

TEST(integerTest, integerOverflowingAbs) {
  for (int32_t i = 0; i <= 1000; i += 10) {
    int32 num = i;
    auto [ret, flag] = num.overflowing_abs();
    ASSERT_FALSE(flag);
    ASSERT_EQ(ret, num);
  }

  for (int32_t i = -1000; i <= 0; i += 10) {
    int32 num = i;
    auto [ret, flag] = num.overflowing_abs();
    ASSERT_FALSE(flag);
    ASSERT_EQ(ret, int32(-i));
  }

  int32 max_num = int32::MAX;
  auto [ret, flag] = max_num.overflowing_abs();
  ASSERT_FALSE(flag);
  ASSERT_EQ(ret, max_num);
  int32 min_num = int32::MIN;
  std::tie(ret, flag) = min_num.overflowing_abs();
  ASSERT_TRUE(flag);
  ASSERT_EQ(ret, min_num);
}

TEST(integerTest, integerOverflowingAbsNoSideEffects) {
  int32 num = 10;
  int32 tmp_num = num;
  ASSERT_EQ(num, tmp_num);

  auto [_, flag] = num.overflowing_abs();
  ASSERT_FALSE(flag);
  ASSERT_EQ(num, tmp_num);
}

TEST(integerTest, integerSaturatingAbs) {
  for (int32_t i = 0; i <= 1000; i += 10) {
    int32 num = i;
    ASSERT_EQ(num.saturating_abs(), num);
  }

  for (int32_t i = -1000; i <= 0; i += 10) {
    int32 num = i;
    ASSERT_EQ(num.saturating_abs(), int32(-i));
  }

  int32 max_num = int32::MAX;
  ASSERT_EQ(max_num.saturating_abs(), max_num);
  int32 min_num = int32::MIN;
  ASSERT_EQ(min_num.saturating_abs(), int32::MAX);
}

TEST(integerTest, integerSaturatingAbsNoSideEffects) {
  int32 num = 10;
  int32 tmp_num = num;
  ASSERT_EQ(num, tmp_num);

  auto _ = num.saturating_abs();
  ASSERT_EQ(num, tmp_num);
}

TEST(integerTest, integerCheckedAbs) {
  for (int32_t i = 0; i <= 1000; i += 10) {
    int32 num = i;
    ASSERT_EQ(num.checked_abs(), num);
  }

  for (int32_t i = -1000; i <= 0; i += 10) {
    int32 num = i;
    ASSERT_EQ(num.checked_abs(), int32(-i));
  }

  int32 max_num = int32::MAX;
  ASSERT_EQ(max_num.checked_abs(), max_num);
  int32 min_num = int32::MIN;
  ASSERT_EQ(min_num.checked_abs(), std::nullopt);
}

TEST(integerTest, integerCheckedAbsNoSideEffects) {
  int32 num = 10;
  int32 tmp_num = num;
  ASSERT_EQ(num, tmp_num);

  auto _ = num.checked_abs();
  ASSERT_EQ(num, tmp_num);
}

TEST(integerTest, integerWrappingAbs) {
  for (int32_t i = 0; i <= 1000; i += 10) {
    int32 num = i;
    ASSERT_EQ(num.wrapping_abs(), num);
  }

  for (int32_t i = -1000; i <= 0; i += 10) {
    int32 num = i;
    ASSERT_EQ(num.wrapping_abs(), int32(-i));
  }

  int32 max_num = int32::MAX;
  ASSERT_EQ(max_num.wrapping_abs(), max_num);
  int32 min_num = int32::MIN;
  ASSERT_EQ(min_num.wrapping_abs(), int32::MIN);
}

TEST(integerTest, integerWrappingAbsNoSideEffects) {
  int32 num = 10;
  int32 tmp_num = num;
  ASSERT_EQ(num, tmp_num);

  auto _ = num.wrapping_abs();
  ASSERT_EQ(num, tmp_num);
}

TEST(integerTest, integerNeg) {
  for (int32_t i = 0; i <= 1000; i += 10) {
    int32 num = i;
    ASSERT_EQ(-num, int32(-i));
  }

  for (int32_t i = -1000; i <= 0; i += 10) {
    int32 num = i;
    ASSERT_EQ(num.abs(), -num);
  }

  int32 max_num = int32::MAX;
  ASSERT_EQ(-max_num, -int32::MAX);
  int32 min_num = int32::MIN;
  ASSERT_THROW(-min_num, std::runtime_error);
}

TEST(integerTest, integerNegNoSideEffects) {
  int32 num = 10;
  int32 tmp_num = num;
  ASSERT_EQ(num, tmp_num);

  auto _ = -num;
  ASSERT_EQ(num, tmp_num);
}

TEST(integerTest, integerCheckedNeg) {
  for (int32_t i = 0; i <= 1000; i += 10) {
    int32 num = i;
    ASSERT_EQ(num.checked_neg(), int32(-i));
  }

  for (int32_t i = -1000; i <= 0; i += 10) {
    int32 num = i;
    ASSERT_EQ(num.checked_neg(), num.checked_abs());
  }

  int32 max_num = int32::MAX;
  ASSERT_EQ(max_num.checked_neg(), int32(-int32::MAX));
  int32 min_num = int32::MIN;
  ASSERT_EQ(min_num.checked_neg(), std::nullopt);
}

TEST(integerTest, integerCheckedNegNoSideEffects) {
  int32 num = 10;
  int32 tmp_num = num;
  ASSERT_EQ(num, tmp_num);

  auto _ = num.checked_neg();
  ASSERT_EQ(num, tmp_num);
}

TEST(integerTest, integerOverflowingNeg) {
  for (int32_t i = 0; i <= 1000; i += 10) {
    int32 num = i;
    auto [ret, flag] = num.overflowing_neg();
    ASSERT_FALSE(flag);
    ASSERT_EQ(ret, int32(-i));
  }

  for (int32_t i = -1000; i <= 0; i += 10) {
    int32 num = i;
    auto [ret, flag] = num.overflowing_neg();
    ASSERT_FALSE(flag);
    ASSERT_EQ(ret, num.checked_abs());
  }

  int32 max_num = int32::MAX;
  auto [ret, flag] = max_num.overflowing_neg();
  ASSERT_FALSE(flag);
  ASSERT_EQ(ret, int32(-int32::MAX));
  int32 min_num = int32::MIN;
  std::tie(ret, flag) = min_num.overflowing_neg();
  ASSERT_TRUE(flag);
  ASSERT_EQ(ret, min_num);
}

TEST(integerTest, integerOverflowingNegNoSideEffects) {
  int32 num = 10;
  int32 tmp_num = num;
  ASSERT_EQ(num, tmp_num);

  auto _ = num.overflowing_neg();
  ASSERT_EQ(num, tmp_num);
}

TEST(integerTest, integerWrappingNeg) {
  for (int32_t i = 0; i <= 1000; i += 10) {
    int32 num = i;
    ASSERT_EQ(num.wrapping_neg(), int32(-i));
  }

  for (int32_t i = -1000; i <= 0; i += 10) {
    int32 num = i;
    ASSERT_EQ(num.wrapping_neg(), num.checked_abs());
  }

  int32 max_num = int32::MAX;
  ASSERT_EQ(max_num.wrapping_neg(), int32(-int32::MAX));
  int32 min_num = int32::MIN;
  ASSERT_EQ(min_num.wrapping_neg(), min_num);
}

TEST(integerTest, integerWrappingNegNoSideEffects) {
  int32 num = 10;
  int32 tmp_num = num;
  ASSERT_EQ(num, tmp_num);

  auto _ = num.wrapping_neg();
  ASSERT_EQ(num, tmp_num);
}

TEST(integerTest, integerSaturatingNeg) {
  for (int32_t i = 0; i <= 1000; i += 10) {
    int32 num = i;
    ASSERT_EQ(num.saturating_neg(), int32(-i));
  }

  for (int32_t i = -1000; i <= 0; i += 10) {
    int32 num = i;
    ASSERT_EQ(num.saturating_neg(), num.checked_abs());
  }

  int32 max_num = int32::MAX;
  ASSERT_EQ(max_num.saturating_neg(), int32(-int32::MAX));
  int32 min_num = int32::MIN;
  ASSERT_EQ(min_num.saturating_neg(), max_num);
}

TEST(integerTest, integerSaturatingNegNoSideEffects) {
  int32 num = 10;
  int32 tmp_num = num;
  ASSERT_EQ(num, tmp_num);

  auto _ = num.saturating_neg();
  ASSERT_EQ(num, tmp_num);
}
