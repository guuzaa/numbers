#include <numeric>
#include <vector>
#include "gtest/gtest.h"

#include "integer.hh"
#include "test/utils.hh"

TEST(IntegerTest, IntegerTemplate) {
  Integer<long> test = 0;
  Integer<short> test1 = 0;
  Integer<long long> test2 = 0;
  Integer<int8_t> test3 = 0;
  Integer<int16_t> test4 = 0;
  Integer<int32_t> test5 = 0;
  Integer<int64_t> test6 = 0;
  Integer<int> test7 = 0;
  Integer<char> test8 = 0;
}

TEST(IntegerTest, IntegerStringStream) {
  for (short n = 1; n < 1000; n++) {
    Int16 num = n;
    std::ostringstream stream;
    stream << num;
    ASSERT_EQ(stream.str(), std::to_string(n));
  }

  for (long long n = 1; n < 1000; n++) {
    Int64 num = n;
    std::ostringstream stream;
    stream << num;
    ASSERT_EQ(stream.str(), std::to_string(n));
  }
}

TEST(IntegerTest, IntegerAdd) {
  Int32 num = 10_i32;
  Int32 num1 = 10_i32;
  ASSERT_EQ(num, num1);

  Int32 num2 = 20_i32;
  Int32 zero_num = 0_i32;
  Int32 max_num = Int32::MAX;
  Int32 min_num = Int32::MIN;

  ASSERT_EQ(num + num1, num2);
  ASSERT_EQ(zero_num + max_num, max_num);
  ASSERT_EQ(max_num + min_num, -1);
}

TEST(IntegerTest, IntegerAddNoSideEffects) {
  Int32 num = 10_i32;
  Int32 num1 = num;
  Int32 num2 = num;
  ASSERT_EQ(num1, num);
  ASSERT_EQ(num2, num);

  ASSERT_EQ(num2 + num1, Int32(20));
  ASSERT_EQ(num1, num);
  ASSERT_EQ(num2, num);
}

TEST(IntegerTest, IntegerWrappingAdd) {
  for (int16_t n = std::numeric_limits<int16_t>::min(); n < std::numeric_limits<int16_t>::max(); n++) {
    Int16 num = n;
    ASSERT_EQ(num.wrapping_add(1), Int16(n + 1));
  }
  Int16 num = Int16::MAX;
  ASSERT_EQ(num.wrapping_add(1), Int16(Int16::MIN));

  constexpr int32_t min = 10;
  constexpr int32_t max = 90000;
  for (int run = 0; run < 100; run++) {
    int32_t a = random_integer(min, max);
    int32_t b = random_integer(min, max);
    Int32 num_a = a;
    Int32 num_b = b;
    ASSERT_EQ(Int32(a + b), num_a + num_b);
    ASSERT_EQ(num_a + num_b, a + b);
  }
}

TEST(IntegerTest, IntegerWrappingAddNoSideEffects) {
  Int32 num = 10_i32;
  Int32 num1 = num;
  Int32 num2 = num;
  ASSERT_EQ(num1, num);
  ASSERT_EQ(num2, num);

  ASSERT_EQ(num2.wrapping_add(num1), Int32(20));
  ASSERT_EQ(num1, num);
  ASSERT_EQ(num2, num);
}

TEST(IntegerTest, IntegerAddByAccumulate) {
  constexpr size_t len = 100;
  std::vector<Integer<long>> nums(len);
  ASSERT_EQ(nums.size(), len);
  std::iota(nums.begin(), nums.end(), Integer<long>(1));
  ASSERT_EQ(std::accumulate(nums.begin(), nums.end(), Integer<long>(0)), Integer<long>(5050));
}

TEST(IntegerTest, IntegerCheckedAdd) {
  constexpr int32_t min = 10;
  constexpr int32_t max = 90000;
  for (int run = 0; run < 100; run++) {
    int32_t a = random_integer(min, max);
    int32_t b = random_integer(min, max);
    Int32 num_a = a;
    Int32 num_b = b;
    ASSERT_EQ(num_a.checked_add(num_b).value(), a + b);
  }

  {
    auto num = 10_i16;
    Integer<short> num1 = Integer<short>::MAX;
    ASSERT_EQ(num.checked_add(num1), std::nullopt);
  }

  {
    auto num = 10_i64;
    Integer<long> num1 = Integer<long>::MAX;
    ASSERT_EQ(num.checked_add(num1), std::nullopt);
  }

  {
    Integer<long> num = -1;
    Integer<long> num1 = Integer<long>::MIN;
    ASSERT_EQ(num.checked_add(num1), std::nullopt);
  }

  {
    Integer<long> num1 = Integer<long>::MIN;
    ASSERT_EQ(num1.checked_add(num1), std::nullopt);
    Integer<long> num2 = Integer<long>::MAX;
    ASSERT_EQ(num2.checked_add(num2), std::nullopt);
  }
}

TEST(IntegerTest, IntegerCheckedAddNoSideEffects) {
  Int32 num = 10_i32;
  Int32 num1 = num;
  Int32 num2 = num;
  ASSERT_EQ(num1, num);
  ASSERT_EQ(num2, num);

  ASSERT_EQ(num2.checked_add(num1).value(), Int32(20));
  ASSERT_EQ(num1, num);
  ASSERT_EQ(num2, num);
}

TEST(IntegerTest, IntegerOverflowingAdd) {
  constexpr int32_t min = 10;
  constexpr int32_t max = 90000;
  for (int run = 0; run < 100; run++) {
    int32_t a = random_integer(min, max);
    int32_t b = random_integer(min, max);
    Int32 num_a = a;
    Int32 num_b = b;
    auto [ret, flag] = num_a.overflowing_add(num_b);
    ASSERT_FALSE(flag);
    ASSERT_EQ(ret, a + b);
  }

  {
    auto num = 10_i16;
    Integer<short> num1 = Integer<short>::MAX;
    auto [ret, flag] = num.overflowing_add(num1);
    ASSERT_TRUE(flag);
    ASSERT_EQ(ret, 10 + Integer<short>::MAX);
  }

  {
    auto num = 10_i64;
    Integer<long> num1 = Integer<long>::MAX;
    auto [_, flag] = num.overflowing_add(num1);
    ASSERT_TRUE(flag);
  }

  {
    Integer<long> num = -1;
    Integer<long> num1 = Integer<long>::MIN;
    auto [_, flag] = num.overflowing_add(num1);
    ASSERT_TRUE(flag);
  }

  {
    Integer<long> num1 = Integer<long>::MIN;
    auto [_, flag1] = num1.overflowing_add(num1);
    ASSERT_TRUE(flag1);
  }

  {
    Integer<long> num2 = Integer<long>::MAX;
    auto [_, flag2] = num2.overflowing_add(num2);
    ASSERT_TRUE(flag2);
  }
}

TEST(IntegerTest, IntegerOverflowingAddNoSideEffects) {
  Int32 num = 10_i32;
  Int32 num1 = num;
  Int32 num2 = num;
  ASSERT_EQ(num1, num);
  ASSERT_EQ(num2, num);

  auto [ret, flag] = num2.overflowing_add(num1);
  ASSERT_EQ(ret, Int32(20));
  ASSERT_FALSE(flag);
  ASSERT_EQ(num1, num);
  ASSERT_EQ(num2, num);
}

TEST(IntegerTest, IntegerSaturatingAdd) {
  constexpr int32_t min = 10;
  constexpr int32_t max = 90000;
  for (int run = 0; run < 100; run++) {
    int32_t a = random_integer(min, max);
    int32_t b = random_integer(min, max);
    Int32 num_a = a;
    Int32 num_b = b;
    auto ret = num_a.saturating_add(num_b);
    ASSERT_EQ(ret, a + b);
  }

  for (int8_t i = 1; i < std::numeric_limits<int8_t>::max(); i++) {
    Int8 num = Int8::MAX;
    auto ret = num.saturating_add(Int8(i));
    EXPECT_EQ(ret, Int8(Int8::MAX)) << static_cast<int>(i) << '\n';
  }

  {
    auto num = 10_i16;
    Integer<short> num1 = Integer<short>::MAX;
    auto ret = num.saturating_add(num1);
    ASSERT_EQ(ret, Int16::MAX);
  }

  {
    auto num = 10_i64;
    Integer<long> num1 = Integer<long>::MAX;
    auto ret = num.saturating_add(num1);
    ASSERT_EQ(ret, Int64::MAX);
  }

  {
    Int64 max_num = Int64::MAX;
    auto ret = max_num.saturating_add(max_num);
    ASSERT_EQ(ret, Int64::MAX);

    Int64 min_num = Int64::MIN;
    ret = min_num.saturating_add(min_num);
    ASSERT_EQ(ret, Int64::MIN);
  }

  {
    Integer<long> num = -1;
    Integer<long> num1 = Integer<long>::MIN;
    auto ret = num.saturating_add(num1);
    ASSERT_EQ(ret, Int64::MIN);
  }
}

TEST(IntegerTest, IntegerSaturatingAddNoSideEffects) {
  Int32 num = 10_i32;
  Int32 num1 = num;
  Int32 num2 = num;
  ASSERT_EQ(num1, num);
  ASSERT_EQ(num2, num);

  auto ret = num2.saturating_add(num1);
  ASSERT_EQ(ret, Int32(20));
  ASSERT_EQ(num1, num);
  ASSERT_EQ(num2, num);
}

TEST(IntegerTest, IntegerAddOverflow) {
  {
    auto num = 10_i16;
    Integer<short> num1 = Integer<short>::MAX;
    ASSERT_THROW(num + num1, std::runtime_error);
  }

  {
    auto num = 10_i64;
    Integer<long> num1 = Integer<long>::MAX;
    ASSERT_THROW(num + num1, std::runtime_error);
  }

  {
    Integer<long> num = -1;
    Integer<long> num1 = Integer<long>::MIN;
    ASSERT_THROW(num + num1, std::runtime_error);
  }
}

TEST(IntegerTest, IntegerAddOverflowByAccumulate) {
  std::vector<Int64> nums(10);
  ASSERT_EQ(nums.size(), 10);
  std::iota(nums.begin(), nums.end(), Int64(Int64::MAX - 11));
  ASSERT_THROW(std::accumulate(nums.begin(), nums.end(), 0_i64), std::runtime_error);
}

TEST(IntegerTest, IntegerSub) {
  auto num = 10_i32;
  auto num1 = 10_i32;
  auto num2 = 20_i32;
  Int32 num3 = Int32::MAX;
  auto num4 = 0_i32;

  ASSERT_EQ(num2 - num1, num);
  ASSERT_EQ(num - num1, num4);
  ASSERT_EQ(num3 - num4, num3);

  {
    Int64 num = -1;
    Int64 num1 = Integer<long>::MIN;
    ASSERT_EQ(num - num1, Int64::MAX);
  }

  constexpr int32_t min = 10;
  constexpr int32_t max = 90000;

  for (int run = 0; run < 100; run++) {
    int32_t a = random_integer(min, max);
    int32_t b = random_integer(min, max);
    Int32 num_a = a;
    Int32 num_b = b;
    ASSERT_EQ(Int32(a - b), num_a - num_b);
    ASSERT_EQ(num_a - num_b, a - b);
  }
}

TEST(IntegerTest, IntegerSubNoSideEffects) {
  Int32 num = 10_i32;
  Int32 num1 = num;
  Int32 num2 = num;
  ASSERT_EQ(num1, num);
  ASSERT_EQ(num2, num);

  ASSERT_EQ(num2 - num1, Int32(0));
  ASSERT_EQ(num1, num);
  ASSERT_EQ(num2, num);
}

TEST(IntegerTest, IntegerSubOverflow) {
  for (short n = 1; n < 1000; n++) {
    Int16 num = n;
    Int16 num1 = Integer<short>::MIN;
    ASSERT_THROW(num1 - num, std::runtime_error);
  }
}

TEST(IntegerTest, IntegerCheckedSub) {
  for (short n = 1; n < 1000; n++) {
    Int16 num = n;
    Int16 num1 = Integer<short>::MIN;
    ASSERT_EQ(num1.checked_sub(num), std::nullopt);
  }

  for (int16_t n = std::numeric_limits<int16_t>::max(); n > std::numeric_limits<int16_t>::min(); n--) {
    Int16 num = n;
    auto ret = num.checked_sub(1);
    ASSERT_EQ(ret.value(), Int16(n - 1));
  }

  Int16 num = Int16::MIN;
  auto ret = num.checked_sub(1);
  ASSERT_EQ(ret, std::nullopt);
}

TEST(IntegerTest, IntegerCheckedSubNoSideEffects) {
  Int32 num = 10_i32;
  Int32 num1 = num;
  Int32 num2 = num;
  ASSERT_EQ(num1, num);
  ASSERT_EQ(num2, num);

  ASSERT_EQ(num2.checked_sub(num1).value(), Int32(0));
  ASSERT_EQ(num1, num);
  ASSERT_EQ(num2, num);
}

TEST(IntegerTest, IntegerOverflowingSub) {
  for (short n = 1; n < 1000; n++) {
    Int16 num = n;
    Int16 num1 = Int16::MIN;
    auto [ret, flag] = num1.overflowing_sub(num);
    ASSERT_TRUE(flag);
    ASSERT_EQ(ret,  Int16::MIN - n);
  }

  for (int16_t n = std::numeric_limits<int16_t>::max(); n > std::numeric_limits<int16_t>::min(); n--) {
    Int16 num = n;
    auto [ret, flag] = num.overflowing_sub(1);
    ASSERT_FALSE(flag);
    ASSERT_EQ(ret, Int16(n - 1));
  }

  Int16 num = Int16::MIN;
  auto [ret, flag] = num.overflowing_sub(1);
  ASSERT_TRUE(flag);
  ASSERT_EQ(ret, Int16::MIN - 1);
}

TEST(IntegerTest, IntegerOverflowingSubNoSideEffects) {
  Int32 num = 10_i32;
  Int32 num1 = num;
  Int32 num2 = num;
  ASSERT_EQ(num1, num);
  ASSERT_EQ(num2, num);

  auto [ret, flag] = num2.overflowing_sub(num1);
  ASSERT_EQ(ret, Int32(0));
  ASSERT_FALSE(flag);
  ASSERT_EQ(num1, num);
  ASSERT_EQ(num2, num);
}

TEST(IntegerTest, IntegerWrappingSub) {
  for (int16_t n = std::numeric_limits<int16_t>::max(); n > std::numeric_limits<int16_t>::min(); n--) {
    Int16 num = n;
    ASSERT_EQ(num.wrapping_sub(1), Int16(n - 1));
  }
  Int16 num = Int16::MIN;
  ASSERT_EQ(num.wrapping_sub(1), Int16(Int16::MAX));
}

TEST(IntegerTest, IntegerWarppingSubNoSideEffects) {
  Int32 num = 10_i32;
  Int32 num1 = num;
  Int32 num2 = num;
  ASSERT_EQ(num1, num);
  ASSERT_EQ(num2, num);

  auto ret = num2.wrapping_sub(num1);
  ASSERT_EQ(ret, Int32(0));
  ASSERT_EQ(num1, num);
  ASSERT_EQ(num2, num);
}

TEST(IntegerTest, IntegerSaturatingSub) {
  for (short n = 1; n <= 1000; n++) {
    Int16 num = n;
    Int16 num1 = Integer<short>::MIN;
    auto ret = num1.saturating_sub(num);
    ASSERT_EQ(ret, Int16::MIN);
  }

  for (int16_t n = std::numeric_limits<int16_t>::max(); n > std::numeric_limits<int16_t>::min(); n--) {
    Int16 num = n;
    auto ret = num.saturating_sub(1);
    ASSERT_EQ(ret, Int16(n - 1));
  }

  Int16 min_num = Int16::MIN;
  auto ret = min_num.saturating_sub(1);
  ASSERT_EQ(ret, Int16::MIN);

  Int16 max_num = Int16::MAX;
  ret = max_num.saturating_sub(min_num);
  ASSERT_EQ(ret, Int16::MAX);

  ret = min_num.saturating_sub(max_num);
  ASSERT_EQ(ret, Int16::MIN);
}

TEST(IntegerTest, IntegerSaturatingSubNoSideEffects) {
  Int32 num = 10_i32;
  Int32 num1 = num;
  Int32 num2 = num;
  ASSERT_EQ(num1, num);
  ASSERT_EQ(num2, num);

  auto ret = num2.saturating_sub(num1);
  ASSERT_EQ(ret, Int32(0));
  ASSERT_EQ(num1, num);
  ASSERT_EQ(num2, num);
}

TEST(IntegerTest, IntegerMul) {
  auto num = 10_i64;
  auto num1 = 100_i64;
  ASSERT_EQ(num1 * num, 1000);
  num = num.MAX;
  num1 = 0_i64;
  ASSERT_EQ(num1 * num, 0);
  num1 = 1_i64;
  ASSERT_EQ(num1 * num, num);

  constexpr int32_t min = 10;
  constexpr int32_t max = 10000;
  for (int run = 0; run < 100; run++) {
    int32_t a = random_integer(min, max);
    int32_t b = random_integer(min, max);
    Int32 num_a = a;
    Int32 num_b = b;
    ASSERT_EQ(Int32(a * b), num_a * num_b);
    ASSERT_EQ(num_a * num_b, a * b);
  }
}

TEST(IntegerTest, IntegerMulNoSideEffects) {
  Int32 num = 10_i32;
  Int32 num1 = num;
  Int32 num2 = num;
  ASSERT_EQ(num1, num);
  ASSERT_EQ(num2, num);

  auto ret = num2 * num1;
  ASSERT_EQ(ret, Int32(100));
  ASSERT_EQ(num1, num);
  ASSERT_EQ(num2, num);
}

TEST(IntegerTest, IntegerMulOverflow) {
  auto num = 10_i8;
  auto num1 = 100_i8;
  ASSERT_THROW(num1 * num, std::runtime_error);
}

TEST(IntegerTest, IntegerCheckedMul) {
  auto num = 10_i8;
  auto num1 = 100_i8;
  ASSERT_EQ(num1.checked_mul(num), std::nullopt);

  constexpr int32_t min = 10;
  constexpr int32_t max = 10000;
  for (int run = 0; run < 100; run++) {
    int32_t a = random_integer(min, max);
    int32_t b = random_integer(min, max);
    Int32 num_a = a;
    Int32 num_b = b;
    ASSERT_EQ(num_a.checked_mul(num_b).value(), a * b);
  }
}

TEST(IntegerTest, IntegerCheckedMulNoSideEffects) {
  Int32 num = 10_i32;
  Int32 num1 = num;
  Int32 num2 = num;
  ASSERT_EQ(num1, num);
  ASSERT_EQ(num2, num);

  auto ret = num2.checked_mul(num1);
  ASSERT_EQ(ret.value(), Int32(100));
  ASSERT_EQ(num1, num);
  ASSERT_EQ(num2, num);
}

TEST(IntegerTest, IntegerOverflowingMul) {
  auto num = 10_i8;
  auto num1 = 100_i8;
  auto [ret, flag] = num1.overflowing_mul(num);
  ASSERT_EQ(ret, int8_t(10 * 100));
  ASSERT_TRUE(flag);

  constexpr int32_t min = 10;
  constexpr int32_t max = 10000;
  for (int run = 0; run < 100; run++) {
    int32_t a = random_integer(min, max);
    int32_t b = random_integer(min, max);
    Int32 num_a = a;
    Int32 num_b = b;
    auto [ret, flag] = num_a.overflowing_mul(num_b);
    ASSERT_FALSE(flag);
    ASSERT_EQ(ret, a * b);
  }
}

TEST(IntegerTest, IntegerOverflowingMulNoSideEffects) {
  Int32 num = 10_i32;
  Int32 num1 = num;
  Int32 num2 = num;
  ASSERT_EQ(num1, num);
  ASSERT_EQ(num2, num);

  auto [ret, flag] = num2.overflowing_mul(num1);
  ASSERT_FALSE(flag);
  ASSERT_EQ(ret, Int32(100));
  ASSERT_EQ(num1, num);
  ASSERT_EQ(num2, num);
}

TEST(IntegerTest, IntegerSaturatingMul) {
  auto num = 10_i8;
  auto num1 = 100_i8;
  auto ret = num1.saturating_mul(num);
  ASSERT_EQ(ret, Int8::MAX);

  Int16 max_num = Int16::MAX;
  ASSERT_EQ(max_num.saturating_mul(max_num), Int16::MAX);
  Int16 min_num = Int16::MIN;
  Int16 num2 = Int16{-30};
  ASSERT_EQ(min_num.saturating_mul(min_num), Int16::MAX);
  ASSERT_EQ(min_num.saturating_mul(num2), Int16::MAX);

  constexpr int32_t min = 10;
  constexpr int32_t max = 10000;
  for (int run = 0; run < 100; run++) {
    int32_t a = random_integer(min, max);
    int32_t b = random_integer(min, max);
    Int32 num_a = a;
    Int32 num_b = b;
    auto ret = num_a.saturating_mul(num_b);
    ASSERT_EQ(ret, a * b);
  }
}

TEST(IntegerTest, IntegerSaturatingMulNoSideEffects) {
  Int32 num = 10_i32;
  Int32 num1 = num;
  Int32 num2 = num;
  ASSERT_EQ(num1, num);
  ASSERT_EQ(num2, num);

  auto ret = num2.saturating_mul(num1);
  ASSERT_EQ(ret, Int32(100));
  ASSERT_EQ(num1, num);
  ASSERT_EQ(num2, num);
}

TEST(IntegerTest, IntegerWrappingMul) {
  Int16 min_num = Int16::MIN;
  Int16 max_num = Int16::MAX;
  Int16 num1 = -1;
  ASSERT_EQ(min_num.wrapping_mul(num1), min_num);
  ASSERT_EQ(max_num.wrapping_mul(num1), min_num + 1);
}

TEST(IntegerTest, IntegerWrappingMulNoSideEffects) {
  Int32 num = 10_i32;
  Int32 num1 = num;
  Int32 num2 = num;
  ASSERT_EQ(num1, num);
  ASSERT_EQ(num2, num);

  auto ret = num2.wrapping_mul(num1);
  ASSERT_EQ(ret, Int32(100));
  ASSERT_EQ(num1, num);
  ASSERT_EQ(num2, num);
}

TEST(IntegerTest, IntegerDiv) {
  constexpr int32_t min = 10;
  constexpr int32_t max = 10000;
  for (int run = 0; run < 100; run++) {
    int32_t a = random_integer(min, max);
    int32_t b = random_integer(min, max);
    Int32 num_a = a;
    Int32 num_b = b;
    ASSERT_EQ(Int32(a / b), num_a / num_b);
    EXPECT_EQ(num_a / num_b, a / b) << a << ' ' << b << '\n';
  }
}

TEST(IntegerTest, IntegerDivNoSideEffects) {
  Int32 num = 10_i32;
  Int32 num1 = num;
  Int32 num2 = num;
  ASSERT_EQ(num1, num);
  ASSERT_EQ(num2, num);

  auto ret = num2 / num1;
  ASSERT_EQ(ret, Int32(1));
  ASSERT_EQ(num1, num);
  ASSERT_EQ(num2, num);
}

TEST(IntegerTest, IntegerDivOverflow) {
  Int8 num = -1;
  Int8 num1 = Int8::MIN;
  ASSERT_THROW(num1 / num, std::runtime_error);
}

TEST(IntegerTest, IntegerWrappingDiv) {
  Int8 num = -1;
  Int8 num1 = Int8::MIN;
  ASSERT_EQ(num1.wrapping_div(num), num1);
}

TEST(IntegerTest, IntegerWrappingDivNoSideEffects) {
  Int32 num = 10_i32;
  Int32 num1 = num;
  Int32 num2 = num;
  ASSERT_EQ(num1, num);
  ASSERT_EQ(num2, num);

  auto ret = num2.wrapping_div(num1);
  ASSERT_EQ(ret, Int32(1));
  ASSERT_EQ(num1, num);
  ASSERT_EQ(num2, num);
}

TEST(IntegerTest, IntegerCheckedDiv) {
  constexpr int32_t min = 10;
  constexpr int32_t max = 10000;
  for (int run = 0; run < 100; run++) {
    int32_t a = random_integer(min, max);
    int32_t b = random_integer(min, max);
    Int32 num_a = a;
    Int32 num_b = b;
    EXPECT_EQ(num_a.checked_div(num_b).value(), a / b);
  }

  Int8 num = -1;
  Int8 num1 = Int8::MIN;
  ASSERT_EQ(num1.checked_div(num), std::nullopt);
}

TEST(IntegerTest, IntegerCheckedDivNoSideEffects) {
  Int32 num = 10_i32;
  Int32 num1 = num;
  Int32 num2 = num;
  ASSERT_EQ(num1, num);
  ASSERT_EQ(num2, num);

  auto ret = num2.checked_div(num1);
  ASSERT_EQ(ret.value(), Int32(1));
  ASSERT_EQ(num1, num);
  ASSERT_EQ(num2, num);
}

TEST(IntegerTest, IntegerOverflowingDiv) {
  constexpr int32_t min = 10;
  constexpr int32_t max = 10000;
  for (int run = 0; run < 100; run++) {
    int32_t a = random_integer(min, max);
    int32_t b = random_integer(min, max);
    Int32 num_a = a;
    Int32 num_b = b;
    auto [ret, flag] = num_a.overflowing_div(num_b);
    ASSERT_FALSE(flag);
    EXPECT_EQ(ret, a / b);
  }

  Int8 num = -1;
  Int8 num1 = Int8::MIN;
  auto [ret, flag] = num1.overflowing_div(num);
  ASSERT_TRUE(flag);
  ASSERT_EQ(ret, -1 * Int8::MIN);
}

TEST(IntegerTest, IntegerOverflowingDivNoSideEffects) {
  Int32 num = 10_i32;
  Int32 num1 = num;
  Int32 num2 = num;
  ASSERT_EQ(num1, num);
  ASSERT_EQ(num2, num);

  auto [ret, flag] = num2.overflowing_div(num1);
  ASSERT_FALSE(flag);
  ASSERT_EQ(num1, num);
  ASSERT_EQ(num2, num);
}

TEST(IntegerTest, IntegerSaturatingDiv) {
  constexpr int32_t min = 10;
  constexpr int32_t max = 10000;
  for (int run = 0; run < 100; run++) {
    int32_t a = random_integer(min, max);
    int32_t b = random_integer(min, max);
    Int32 num_a = a;
    Int32 num_b = b;
    auto ret = num_a.saturating_div(num_b);
    EXPECT_EQ(ret, a / b);
  }

  Int8 num = -1;
  Int8 num1 = Int8::MIN;
  auto ret = num1.saturating_div(num);
  ASSERT_EQ(ret, Int8::MIN);
}

TEST(IntegerTest, IntegerSaturatingDivNoSideEffects) {
  Int32 num = 10_i32;
  Int32 num1 = num;
  Int32 num2 = num;
  ASSERT_EQ(num1, num);
  ASSERT_EQ(num2, num);

  auto ret = num2.saturating_div(num1);
  ASSERT_EQ(num1, num);
  ASSERT_EQ(num2, num);
}

TEST(IntegerTest, IntegerAbs) {
  for (int32_t i = 0; i <= 1000; i += 10) {
    Int32 num = i;
    ASSERT_EQ(num.abs(), num);
  }

  for (int32_t i = -1000; i <= 0; i += 10) {
    Int32 num = i;
    ASSERT_EQ(num.abs(), Int32(-i));
  }

  Int32 max_num = Int32::MAX;
  ASSERT_EQ(max_num.abs(), max_num);
  Int32 min_num = Int32::MIN;
  ASSERT_THROW(min_num.abs(), std::runtime_error);
}

TEST(IntegerTest, IntegerAbsNoSideEffects) {
  Int32 num = 10_i32;
  Int32 tmp_num = num;
  ASSERT_EQ(num, tmp_num);

  auto _ = num.abs();
  ASSERT_EQ(num, tmp_num);
}

TEST(IntegerTest, IntegerOverflowingAbs) {
  for (int32_t i = 0; i <= 1000; i += 10) {
    Int32 num = i;
    auto [ret, flag] = num.overflowing_abs();
    ASSERT_FALSE(flag);
    ASSERT_EQ(ret, num);
  }

  for (int32_t i = -1000; i <= 0; i += 10) {
    Int32 num = i;
    auto [ret, flag] = num.overflowing_abs();
    ASSERT_FALSE(flag);
    ASSERT_EQ(ret, Int32(-i));
  }

  Int32 max_num = Int32::MAX;
  auto [ret, flag] = max_num.overflowing_abs();
  ASSERT_FALSE(flag);
  ASSERT_EQ(ret, max_num);
  Int32 min_num = Int32::MIN;
  std::tie(ret, flag) = min_num.overflowing_abs();
  ASSERT_TRUE(flag);
  ASSERT_EQ(ret, min_num);
}

TEST(IntegerTest, IntegerOverflowingAbsNoSideEffects) {
  Int32 num = 10_i32;
  Int32 tmp_num = num;
  ASSERT_EQ(num, tmp_num);

  auto [_, flag] = num.overflowing_abs();
  ASSERT_FALSE(flag);
  ASSERT_EQ(num, tmp_num);
}

TEST(IntegerTest, IntegerSaturatingAbs) {
  for (int32_t i = 0; i <= 1000; i += 10) {
    Int32 num = i;
    ASSERT_EQ(num.saturating_abs(), num);
  }

  for (int32_t i = -1000; i <= 0; i += 10) {
    Int32 num = i;
    ASSERT_EQ(num.saturating_abs(), Int32(-i));
  }

  Int32 max_num = Int32::MAX;
  ASSERT_EQ(max_num.saturating_abs(), max_num);
  Int32 min_num = Int32::MIN;
  ASSERT_EQ(min_num.saturating_abs(), Integer(Int32::MAX));
}

TEST(IntegerTest, IntegerSaturatingAbsNoSideEffects) {
  Int32 num = 10_i32;
  Int32 tmp_num = num;
  ASSERT_EQ(num, tmp_num);

  auto _ = num.saturating_abs();
  ASSERT_EQ(num, tmp_num);
}

TEST(IntegerTest, IntegerCheckedAbs) {
  for (int32_t i = 0; i <= 1000; i += 10) {
    Int32 num = i;
    ASSERT_EQ(num.checked_abs(), num);
  }

  for (int32_t i = -1000; i <= 0; i += 10) {
    Int32 num = i;
    ASSERT_EQ(num.checked_abs(), Int32(-i));
  }

  Int32 max_num = Int32::MAX;
  ASSERT_EQ(max_num.checked_abs(), max_num);
  Int32 min_num = Int32::MIN;
  ASSERT_EQ(min_num.checked_abs(), std::nullopt);
}

TEST(IntegerTest, IntegerCheckedAbsNoSideEffects) {
  Int32 num = 10_i32;
  Int32 tmp_num = num;
  ASSERT_EQ(num, tmp_num);

  auto _ = num.checked_abs();
  ASSERT_EQ(num, tmp_num);
}

TEST(IntegerTest, IntegerWrappingAbs) {
  for (int32_t i = 0; i <= 1000; i += 10) {
    Int32 num = i;
    ASSERT_EQ(num.wrapping_abs(), num);
  }

  for (int32_t i = -1000; i <= 0; i += 10) {
    Int32 num = i;
    ASSERT_EQ(num.wrapping_abs(), Int32(-i));
  }

  Int32 max_num = Int32::MAX;
  ASSERT_EQ(max_num.wrapping_abs(), max_num);
  Int32 min_num = Int32::MIN;
  ASSERT_EQ(min_num.wrapping_abs(), Int32(Int32::MIN));
}

TEST(IntegerTest, IntegerWrappingAbsNoSideEffects) {
  Int32 num = 10_i32;
  Int32 tmp_num = num;
  ASSERT_EQ(num, tmp_num);

  auto _ = num.wrapping_abs();
  ASSERT_EQ(num, tmp_num);
}

TEST(IntegerTest, IntegerNeg) {
  for (int32_t i = 0; i <= 1000; i += 10) {
    Int32 num = i;
    ASSERT_EQ(-num, Integer(-i));
  }

  for (int32_t i = -1000; i <= 0; i += 10) {
    Int32 num = i;
    ASSERT_EQ(num.abs(), -num);
  }

  Int32 max_num = Int32::MAX;
  ASSERT_EQ(-max_num, Int32(-Int32::MAX));
  Int32 min_num = Int32::MIN;
  ASSERT_THROW(-min_num, std::runtime_error);
}

TEST(IntegerTest, IntegerNegNoSideEffects) {
  Int32 num = 10_i32;
  Int32 tmp_num = num;
  ASSERT_EQ(num, tmp_num);

  auto _ = -num;
  ASSERT_EQ(num, tmp_num);
}

TEST(IntegerTest, IntegerCheckedNeg) {
  for (int32_t i = 0; i <= 1000; i += 10) {
    Int32 num = i;
    ASSERT_EQ(num.checked_neg(), Integer(-i));
  }

  for (int32_t i = -1000; i <= 0; i += 10) {
    Int32 num = i;
    ASSERT_EQ(num.checked_neg(), num.checked_abs());
  }

  Int32 max_num = Int32::MAX;
  ASSERT_EQ(max_num.checked_neg(), Int32(-Int32::MAX));
  Int32 min_num = Int32::MIN;
  ASSERT_EQ(min_num.checked_neg(), std::nullopt);
}

TEST(IntegerTest, IntegerCheckedNegNoSideEffects) {
  Int32 num = 10_i32;
  Int32 tmp_num = num;
  ASSERT_EQ(num, tmp_num);

  auto _ = num.checked_neg();
  ASSERT_EQ(num, tmp_num);
}

TEST(IntegerTest, IntegerOverflowingNeg) {
  for (int32_t i = 0; i <= 1000; i += 10) {
    Int32 num = i;
    auto [ret, flag] = num.overflowing_neg();
    ASSERT_FALSE(flag);
    ASSERT_EQ(ret, Integer(-i));
  }

  for (int32_t i = -1000; i <= 0; i += 10) {
    Int32 num = i;
    auto [ret, flag] = num.overflowing_neg();
    ASSERT_FALSE(flag);
    ASSERT_EQ(ret, num.checked_abs());
  }

  Int32 max_num = Int32::MAX;
  auto [ret, flag] = max_num.overflowing_neg();
  ASSERT_FALSE(flag);
  ASSERT_EQ(ret, Int32(-Int32::MAX));
  Int32 min_num = Int32::MIN;
  std::tie(ret, flag) = min_num.overflowing_neg();
  ASSERT_TRUE(flag);
  ASSERT_EQ(ret, min_num);
}

TEST(IntegerTest, IntegerOverflowingNegNoSideEffects) {
  Int32 num = 10_i32;
  Int32 tmp_num = num;
  ASSERT_EQ(num, tmp_num);

  auto _ = num.overflowing_neg();
  ASSERT_EQ(num, tmp_num);
}

TEST(IntegerTest, IntegerWrappingNeg) {
  for (int32_t i = 0; i <= 1000; i += 10) {
    Int32 num = i;
    ASSERT_EQ(num.wrapping_neg(), Integer(-i));
  }

  for (int32_t i = -1000; i <= 0; i += 10) {
    Int32 num = i;
    ASSERT_EQ(num.wrapping_neg(), num.checked_abs());
  }

  Int32 max_num = Int32::MAX;
  ASSERT_EQ(max_num.wrapping_neg(), Int32(-Int32::MAX));
  Int32 min_num = Int32::MIN;
  ASSERT_EQ(min_num.wrapping_neg(), min_num);
}

TEST(IntegerTest, IntegerWrappingNegNoSideEffects) {
  Int32 num = 10_i32;
  Int32 tmp_num = num;
  ASSERT_EQ(num, tmp_num);

  auto _ = num.wrapping_neg();
  ASSERT_EQ(num, tmp_num);
}

TEST(IntegerTest, IntegerSaturatingNeg) {
  for (int32_t i = 0; i <= 1000; i += 10) {
    Int32 num = i;
    ASSERT_EQ(num.saturating_neg(), Integer(-i));
  }

  for (int32_t i = -1000; i <= 0; i += 10) {
    Int32 num = i;
    ASSERT_EQ(num.saturating_neg(), num.checked_abs());
  }

  Int32 max_num = Int32::MAX;
  ASSERT_EQ(max_num.saturating_neg(), Int32(-Int32::MAX));
  Int32 min_num = Int32::MIN;
  ASSERT_EQ(min_num.saturating_neg(), max_num);
}

TEST(IntegerTest, IntegerSaturatingNegNoSideEffects) {
  Int32 num = 10_i32;
  Int32 tmp_num = num;
  ASSERT_EQ(num, tmp_num);

  auto _ = num.saturating_neg();
  ASSERT_EQ(num, tmp_num);
}
