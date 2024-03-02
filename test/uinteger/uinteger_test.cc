#include <numeric>

#include "gtest/gtest.h"

#include "uinteger.hh"
#include "test/utils.hh"

using namespace numbers;

TEST(UintegerTest, UintegerMinMax) {
  ASSERT_THROW(-u64::MAX, std::runtime_error);
  ASSERT_EQ(-u16::MIN, u16::MIN);
  ASSERT_EQ(u16::MAX.saturating_add(u16::MAX), u16::MAX);
}

TEST(UintegerTest, UintegerStringStream) {
  for (unsigned short n = 0; n <= 1000; n++) {
    u16 num = n;
    std::ostringstream stream;
    stream << num;
    ASSERT_EQ(stream.str(), std::to_string(n));
  }

  for (unsigned long long n = 0; n < 1000; n++) {
    u64 num = n;
    std::ostringstream stream;
    stream << num;
    ASSERT_EQ(stream.str(), std::to_string(n));
  }
}

TEST(UintegerTest, UintegerAdd) {
  u32 num = 10;
  u32 num1 = 10;
  ASSERT_EQ(num, num1);

  u32 num2 = 20;
  u32 zero_num = 0;
  u32 max_num = u32::MAX;
  u32 min_num = u32::MIN;

  ASSERT_EQ(zero_num, min_num);
  ASSERT_EQ(num + num1, num2);
  ASSERT_EQ(zero_num + max_num, max_num);
  ASSERT_EQ(max_num + min_num, max_num);
}

TEST(UintegerTest, UintegerAddNoSideEffects) {
  u32 num = 10;
  u32 num1 = num;
  u32 num2 = num;
  ASSERT_EQ(num1, num);
  ASSERT_EQ(num2, num);

  ASSERT_EQ(num2 + num1, 20);
  ASSERT_EQ(num1, num);
  ASSERT_EQ(num2, num);
}

TEST(UintegerTest, UintegerWrappingAdd) {
  for (uint16_t n = std::numeric_limits<uint16_t>::min(); n < std::numeric_limits<uint16_t>::max(); n++) {
    u16 num = n;
    ASSERT_EQ(num.wrapping_add(1), u16(n + 1));
  }
  u16 num = u16::MAX;
  ASSERT_EQ(num.wrapping_add(1), u16(u16::MIN));
}

TEST(UintegerTest, UintegerWrappingAddNoSideEffects) {
  u32 num = 10;
  u32 num1 = num;
  u32 num2 = num;
  ASSERT_EQ(num1, num);
  ASSERT_EQ(num2, num);

  ASSERT_EQ(num2.wrapping_add(num1), 20);
  ASSERT_EQ(num1.wrapping_add(num2), 20);
  ASSERT_EQ(num1, num);
  ASSERT_EQ(num2, num);
}

TEST(UintegerTest, UintegerAddByAccumulate) {
  constexpr size_t len = 100;
  std::vector<u64> nums(len);
  ASSERT_EQ(nums.size(), len);
  std::iota(nums.begin(), nums.end(), u64(1));
  ASSERT_EQ(std::accumulate(nums.begin(), nums.end(), u64(0)), u64(5050));
}

TEST(UintegerTest, UintegerCheckedAdd) {
  constexpr uint32_t min = 10;
  constexpr uint32_t max = 90000;
  for (int run = 0; run < 100; run++) {
    uint32_t a = random_integer(min, max);
    uint32_t b = random_integer(min, max);
    u32 num_a = a;
    u32 num_b = b;
    ASSERT_EQ(num_a.checked_add(num_b).value(), a + b);
  }

  {
    u16 num = 10;
    u16 num1 = u16::MAX;
    ASSERT_EQ(num.checked_add(num1), std::nullopt);
  }

  {
    u64 num = 10;
    u64 num1 = u64::MAX;
    ASSERT_EQ(num.checked_add(num1), std::nullopt);
  }

  {
    Uinteger<unsigned long> num1 = Uinteger<unsigned long>::MIN;
    ASSERT_EQ(num1.checked_add(num1).value(), num1);
    Uinteger<unsigned long> num2 = Uinteger<unsigned long>::MAX;
    ASSERT_EQ(num2.checked_add(num2), std::nullopt);
  }
}

TEST(UintegerTest, UintegerCheckedAddNoSideEffects) {
  u32 num = 10;
  u32 num1 = num;
  u32 num2 = num;
  ASSERT_EQ(num1, num);
  ASSERT_EQ(num2, num);

  ASSERT_EQ(num2.checked_add(num1).value(), 20);
  ASSERT_EQ(num1.checked_add(num2).value(), 20);
  ASSERT_EQ(num1, num);
  ASSERT_EQ(num2, num);
}

TEST(UintegerTest, UintegerOverflowingAdd) {
  constexpr uint32_t min = 10;
  constexpr uint32_t max = 90000;
  for (int run = 0; run < 100; run++) {
    uint32_t a = random_integer(min, max);
    uint32_t b = random_integer(min, max);
    u32 num_a = a;
    u32 num_b = b;
    auto [ret, flag] = num_a.overflowing_add(num_b);
    ASSERT_FALSE(flag);
    ASSERT_EQ(ret, a + b);
  }

  {
    u16 num = 10;
    Uinteger<unsigned short> num1 = Uinteger<unsigned short>::MAX;
    auto [ret, flag] = num.overflowing_add(num1);
    ASSERT_TRUE(flag);
    ASSERT_EQ(ret, 10 + std::numeric_limits<unsigned short>::max());
  }

  {
    u64 num = 10;
    u64 num1 = u64::MAX;
    auto [_, flag] = num.overflowing_add(num1);
    ASSERT_TRUE(flag);
  }

  {
    Uinteger<unsigned long> num1 = Uinteger<unsigned long>::MIN;
    auto [ret, flag1] = num1.overflowing_add(num1);
    ASSERT_FALSE(flag1);
    ASSERT_EQ(ret, num1);
  }

  {
    Uinteger<unsigned long> num2 = Uinteger<unsigned long>::MAX;
    auto [_, flag2] = num2.overflowing_add(num2);
    ASSERT_TRUE(flag2);
  }
}

TEST(UintegerTest, UintegerOverflowingAddNoSideEffects) {
  u32 num = 10;
  u32 num1 = num;
  u32 num2 = num;
  ASSERT_EQ(num1, num);
  ASSERT_EQ(num2, num);

  auto [ret, flag] = num2.overflowing_add(num1);
  ASSERT_EQ(ret, 20);
  ASSERT_FALSE(flag);
  ASSERT_EQ(num1, num);
  ASSERT_EQ(num2, num);
}

TEST(UintegerTest, UintegerSaturatingAdd) {
  constexpr uint32_t min = 10;
  constexpr uint32_t max = 90000;
  for (int run = 0; run < 100; run++) {
    uint32_t a = random_integer(min, max);
    uint32_t b = random_integer(min, max);
    u32 num_a = a;
    u32 num_b = b;
    auto ret = num_a.saturating_add(num_b);
    ASSERT_EQ(ret, a + b);
  }

  for (uint8_t i = 1; i < std::numeric_limits<uint8_t>::max(); i++) {
    u8 num = u8::MAX;
    auto ret = num.saturating_add(u8(i));
    ASSERT_EQ(ret, u8(u8::MAX));
  }

  {
    u64 max_num = u64::MAX;
    auto ret = max_num.saturating_add(max_num);
    ASSERT_EQ(ret, u64::MAX);

    u64 min_num = u64::MIN;
    ret = min_num.saturating_add(min_num);
    ASSERT_EQ(ret, u64::MIN);
  }
}

TEST(UintegerTest, UintegerSaturatingAddNoSideEffects) {
  u32 num = 10;
  u32 num1 = num;
  u32 num2 = num;
  ASSERT_EQ(num1, num);
  ASSERT_EQ(num2, num);

  auto ret = num2.saturating_add(num1);
  ASSERT_EQ(ret, 20);
  ASSERT_EQ(num1, num);
  ASSERT_EQ(num2, num);
}

TEST(UintegerTest, UintegerAddOverflow) {
  {
    u16 num = 10;
    u16 num1 = u16::MAX;
    ASSERT_THROW(num + num1, std::runtime_error);
  }

  {
    u64 num = 10;
    u64 num1 = u64::MAX;
    ASSERT_THROW(num + num1, std::runtime_error);
  }
}

TEST(UintegerTest, UintegerAddOverflowByAccumulate) {
  std::vector<u64> nums(10);
  ASSERT_EQ(nums.size(), 10);
  std::iota(nums.begin(), nums.end(), u64(u64::MAX - 11));
  ASSERT_THROW(std::accumulate(nums.begin(), nums.end(), u64(0)), std::runtime_error);
}

TEST(UintegerTest, UintegerSub) {
  auto num = 10;
  auto num1 = 10;
  auto num2 = 20;
  u32 num3 = u32::MAX;
  auto num4 = 0;

  ASSERT_EQ(num2 - num1, num);
  ASSERT_EQ(num - num1, num4);
  ASSERT_EQ(num3 - num4, num3);

  constexpr uint32_t min = 10;
  constexpr uint32_t max = 90000;

  for (int run = 0; run < 100; run++) {
    uint32_t a = random_integer(min, max) + max;
    uint32_t b = random_integer(min, max);
    u32 num_a = a;
    u32 num_b = b;
    ASSERT_EQ(u32(a - b), num_a - num_b);
    ASSERT_EQ(num_a - num_b, a - b);
  }
}

TEST(UintegerTest, UintegerSubNoSideEffects) {
  u32 num = 10;
  u32 num1 = num;
  u32 num2 = num;
  ASSERT_EQ(num1, num);
  ASSERT_EQ(num2, num);

  ASSERT_EQ(num2 - num1, 0);
  ASSERT_EQ(num1, num);
  ASSERT_EQ(num2, num);
}

TEST(UintegerTest, UintegerSubOverflow) {
  for (short n = 1; n < 1000; n++) {
    u16 num = n;
    u16 num1 = u16::MIN;
    ASSERT_THROW(num1 - num, std::runtime_error);
  }
}

TEST(UintegerTest, UintegerCheckedSub) {
  for (short n = 1; n < 1000; n++) {
    u16 num = n;
    u16 num1 = u16::MIN;
    ASSERT_EQ(num1.checked_sub(num), std::nullopt);
  }

  for (uint16_t n = std::numeric_limits<uint16_t>::max(); n > std::numeric_limits<uint16_t>::min(); n--) {
    u16 num = n;
    auto ret = num.checked_sub(1);
    ASSERT_EQ(ret.value(), u16(n - 1));
  }
}

TEST(UintegerTest, UintegerCheckedSubNoSideEffects) {
  u32 num = 10;
  u32 num1 = num;
  u32 num2 = num;
  ASSERT_EQ(num1, num);
  ASSERT_EQ(num2, num);

  ASSERT_EQ(num2.checked_sub(num1).value(), 0);
  ASSERT_EQ(num1, num);
  ASSERT_EQ(num2, num);
}

TEST(UintegerTest, UintegerOverflowingSub) {
  for (unsigned short n = 1; n < 1000; n++) {
    u16 num = n;
    u16 num1 = u16::MIN;
    auto [ret, flag] = num1.overflowing_sub(num);
    ASSERT_TRUE(flag);
    ASSERT_EQ(ret, std::numeric_limits<uint16_t>::min() - n);
  }

  for (uint16_t n = std::numeric_limits<uint16_t>::max(); n > std::numeric_limits<uint16_t>::min(); n--) {
    u16 num = n;
    auto [ret, flag] = num.overflowing_sub(1);
    ASSERT_FALSE(flag);
    ASSERT_EQ(ret, n - 1);
  }
}

TEST(UintegerTest, UintegerOverflowingSubNoSideEffects) {
  u32 num = 10;
  u32 num1 = num;
  u32 num2 = num;
  ASSERT_EQ(num1, num);
  ASSERT_EQ(num2, num);

  auto [ret, flag] = num2.overflowing_sub(num1);
  ASSERT_EQ(ret, 0);
  ASSERT_FALSE(flag);
  ASSERT_EQ(num1, num);
  ASSERT_EQ(num2, num);
}

TEST(UintegerTest, UintegerWrappingSub) {
  for (uint16_t n = std::numeric_limits<uint16_t>::max(); n > std::numeric_limits<uint16_t>::min(); n--) {
    u16 num = n;
    ASSERT_EQ(num.wrapping_sub(1), u16(n - 1));
  }

  u16 num = u16::MIN;
  ASSERT_EQ(num.wrapping_sub(1), u16::MAX);
}

TEST(UintegerTest, UintegerWarppingSubNoSideEffects) {
  u32 num = 10;
  u32 num1 = num;
  u32 num2 = num;
  ASSERT_EQ(num1, num);
  ASSERT_EQ(num2, num);

  auto ret = num2.wrapping_sub(num1);
  ASSERT_EQ(ret, 0);
  ASSERT_EQ(num1, num);
  ASSERT_EQ(num2, num);
}

TEST(UintegerTest, UintegerSaturatingSub) {
  for (unsigned short n = 1; n <= 1000; n++) {
    u16 num = n;
    u16 num1 = u16::MIN;
    auto ret = num1.saturating_sub(num);
    ASSERT_EQ(ret, u16::MIN);
  }

  for (uint16_t n = std::numeric_limits<uint16_t>::max(); n > std::numeric_limits<uint16_t>::min(); n--) {
    u16 num = n;
    auto ret = num.saturating_sub(1);
    ASSERT_EQ(ret, u16(n - 1));
  }

  u16 max_num = u16::MAX;
  u16 min_num = u16::MIN;
  auto ret = max_num.saturating_sub(min_num);
  ASSERT_EQ(ret, u16::MAX);
  ret = min_num.saturating_sub(max_num);
  ASSERT_EQ(ret, u16::MIN);
}

TEST(UintegerTest, UintegerSaturatingSubNoSideEffects) {
  u32 num = 10;
  u32 num1 = num;
  u32 num2 = num;
  ASSERT_EQ(num1, num);
  ASSERT_EQ(num2, num);

  auto ret = num2.saturating_sub(num1);
  ASSERT_EQ(ret, 0);
  ASSERT_EQ(num1, num);
  ASSERT_EQ(num2, num);
}

TEST(UintegerTest, UintegerMul) {
  u64 num = 10;
  u64 num1 = 100;
  ASSERT_EQ(num1 * num, 1000);
  num = u64::MAX;
  num1 = 0;
  ASSERT_EQ(num1 * num, 0);
  num1 = 1;
  ASSERT_EQ(num1 * num, num);

  constexpr uint32_t min = 10;
  constexpr uint32_t max = 10000;
  for (int run = 0; run < 100; run++) {
    uint32_t a = random_integer(min, max);
    uint32_t b = random_integer(min, max);
    u32 num_a = a;
    u32 num_b = b;
    ASSERT_EQ(u32(a * b), num_a * num_b);
    ASSERT_EQ(num_a * num_b, a * b);
  }
}

TEST(UintegerTest, UintegerMulNoSideEffects) {
  u32 num = 10;
  u32 num1 = num;
  u32 num2 = num;
  ASSERT_EQ(num1, num);
  ASSERT_EQ(num2, num);

  auto ret = num2 * num1;
  ASSERT_EQ(ret, 100);
  ASSERT_EQ(num1, num);
  ASSERT_EQ(num2, num);
}

TEST(UintegerTest, UintegerCheckedMul) {
  u8 num = 10;
  u8 num1 = 100;
  ASSERT_EQ(num1.checked_mul(num), std::nullopt);

  constexpr uint32_t min = 10;
  constexpr uint32_t max = 10000;
  for (int run = 0; run < 100; run++) {
    uint32_t a = random_integer(min, max);
    uint32_t b = random_integer(min, max);
    u32 num_a = a;
    u32 num_b = b;
    ASSERT_EQ(num_a.checked_mul(num_b).value(), a * b);
  }
}

TEST(UintegerTest, UintegerCheckedMulNoSideEffects) {
  u32 num = 10;
  u32 num1 = num;
  u32 num2 = num;
  ASSERT_EQ(num1, num);
  ASSERT_EQ(num2, num);

  auto ret = num2.checked_mul(num1);
  ASSERT_EQ(ret.value(), 100);
  ASSERT_EQ(num1, num);
  ASSERT_EQ(num2, num);
}

TEST(UintegerTest, UintegerOverflowingMul) {
  u8 num = 10;
  u8 num1 = 100;
  auto [ret, flag] = num1.overflowing_mul(num);
  ASSERT_EQ(ret, uint8_t(10 * 100));
  ASSERT_TRUE(flag);

  constexpr uint32_t min = 10;
  constexpr uint32_t max = 10000;
  for (int run = 0; run < 100; run++) {
    uint32_t a = random_integer(min, max);
    uint32_t b = random_integer(min, max);
    u32 num_a = a;
    u32 num_b = b;
    auto [ret, flag] = num_a.overflowing_mul(num_b);
    ASSERT_FALSE(flag);
    ASSERT_EQ(ret, a * b);
  }
}

TEST(UntegerTest, UintegerOverflowingMulNoSideEffects) {
  u32 num = 10;
  u32 num1 = num;
  u32 num2 = num;
  ASSERT_EQ(num1, num);
  ASSERT_EQ(num2, num);

  auto [ret, flag] = num2.overflowing_mul(num1);
  ASSERT_FALSE(flag);
  ASSERT_EQ(ret, 100);
  ASSERT_EQ(num1, num);
  ASSERT_EQ(num2, num);
}

TEST(UintegerTest, UintegerSaturatingMul) {
  u8 num = 10;
  u8 num1 = 100;
  u8 ret = num1.saturating_mul(num);
  ASSERT_EQ(ret, u8::MAX);

  u16 max_num = u16::MAX;
  ASSERT_EQ(max_num.saturating_mul(max_num), u16::MAX);
  u16 min_num = u16::MIN;
  ASSERT_EQ(min_num.saturating_mul(min_num), min_num);

  constexpr uint32_t min = 10;
  constexpr uint32_t max = 10000;
  for (int run = 0; run < 100; run++) {
    uint32_t a = random_integer(min, max);
    uint32_t b = random_integer(min, max);
    u32 num_a = a;
    u32 num_b = b;
    auto ret = num_a.saturating_mul(num_b);
    ASSERT_EQ(ret, a * b);
  }
}

TEST(UintegerTest, UintegerSaturatingMulNoSideEffects) {
  u32 num = 10;
  u32 num1 = num;
  u32 num2 = num;
  ASSERT_EQ(num1, num);
  ASSERT_EQ(num2, num);

  auto ret = num2.saturating_mul(num1);
  ASSERT_EQ(ret, 100);
  ASSERT_EQ(num1, num);
  ASSERT_EQ(num2, num);
}

TEST(UintegerTest, UintegerDiv) {
  constexpr uint32_t min = 10;
  constexpr uint32_t max = 10000;
  for (int run = 0; run < 100; run++) {
    uint32_t a = random_integer(min, max);
    uint32_t b = random_integer(min, max);
    u32 num_a = a;
    u32 num_b = b;
    ASSERT_EQ(u32(a / b), num_a / num_b);
    ASSERT_EQ(num_a / num_b, a / b);
  }
}

TEST(UintegerTest, UintegerDivNoSideEffects) {
  u32 num = 10;
  u32 num1 = num;
  u32 num2 = num;
  ASSERT_EQ(num1, num);
  ASSERT_EQ(num2, num);

  auto ret = num2 / num1;
  ASSERT_EQ(ret, 1);
  ASSERT_EQ(num1, num);
  ASSERT_EQ(num2, num);
}

TEST(UintegerTest, UintegerWrappingDiv) {
  u8 num = 10;
  u8 num1 = u8::MIN;
  ASSERT_EQ(num1.wrapping_div(num), num1);
}

TEST(UintegerTest, UintegerWrappingDivNoSideEffects) {
  u32 num = 10;
  u32 num1 = num;
  u32 num2 = num;
  ASSERT_EQ(num1, num);
  ASSERT_EQ(num2, num);

  auto ret = num2.wrapping_div(num1);
  ASSERT_EQ(ret, u32(1));
  ASSERT_EQ(num1, num);
  ASSERT_EQ(num2, num);
}

TEST(UintegerTest, UintegerCheckedDiv) {
  constexpr uint32_t min = 10;
  constexpr uint32_t max = 10000;
  for (int run = 0; run < 100; run++) {
    uint32_t a = random_integer(min, max);
    uint32_t b = random_integer(min, max);
    u32 num_a = a;
    u32 num_b = b;
    EXPECT_EQ(num_a.checked_div(num_b).value(), a / b);
  }
}

TEST(UintegerTest, UintegerCheckedDivNoSideEffects) {
  u32 num = 10;
  u32 num1 = num;
  u32 num2 = num;
  ASSERT_EQ(num1, num);
  ASSERT_EQ(num2, num);

  auto ret = num2.checked_div(num1);
  ASSERT_EQ(ret.value(), 1);
  ASSERT_EQ(num1, num);
  ASSERT_EQ(num2, num);
}

TEST(UintegerTest, UintegerOverflowingDiv) {
  constexpr uint32_t min = 10;
  constexpr uint32_t max = 10000;
  for (int run = 0; run < 100; run++) {
    uint32_t a = random_integer(min, max);
    uint32_t b = random_integer(min, max);
    u32 num_a = a;
    u32 num_b = b;
    auto [ret, flag] = num_a.overflowing_div(num_b);
    ASSERT_FALSE(flag);
    EXPECT_EQ(ret, a / b);
  }
}

TEST(UintegerTest, UintegerOverflowingDivNoSideEffects) {
  u32 num = 10;
  u32 num1 = num;
  u32 num2 = num;
  ASSERT_EQ(num1, num);
  ASSERT_EQ(num2, num);

  auto [ret, flag] = num2.overflowing_div(num1);
  ASSERT_FALSE(flag);
  ASSERT_EQ(num1, num);
  ASSERT_EQ(num2, num);
}

TEST(UintegerTest, UintegerSaturatingDiv) {
  constexpr uint32_t min = 10;
  constexpr uint32_t max = 10000;
  for (int run = 0; run < 100; run++) {
    uint32_t a = random_integer(min, max);
    uint32_t b = random_integer(min, max);
    u32 num_a = a;
    u32 num_b = b;
    auto ret = num_a.saturating_div(num_b);
    EXPECT_EQ(ret, a / b);
  }
}

TEST(UintegerTest, UintegerSaturatingDivNoSideEffects) {
  u32 num = 10;
  u32 num1 = num;
  u32 num2 = num;
  ASSERT_EQ(num1, num);
  ASSERT_EQ(num2, num);

  auto ret = num2.saturating_div(num1);
  ASSERT_EQ(num1, num);
  ASSERT_EQ(num2, num);
}

TEST(UintegerTest, UintegerNeg) {
  u32 num = 0;
  ASSERT_EQ(-num, num);

  for (uint32_t i = 1; i <= 1000; i += 10) {
    num = i;
    ASSERT_THROW(-num, std::runtime_error);
  }
}

TEST(UintegerTest, UintegerNegNoSideEffects) {
  u32 num = 10;
  u32 tmp_num = num;
  ASSERT_EQ(num, tmp_num);

  ASSERT_THROW(-num, std::runtime_error);
  ASSERT_EQ(num, tmp_num);
}

TEST(UintegerTest, UintegerCheckedNeg) {
  for (uint32_t i = 1; i <= 1000; i += 10) {
    u32 num = i;
    ASSERT_EQ(num.checked_neg(), std::nullopt);
  }

  u32 min_num = u32::MIN;
  ASSERT_EQ(min_num.checked_neg(), min_num);
}

TEST(UintegerTest, UintegerCheckedNegNoSideEffects) {
  u32 num = 10;
  u32 tmp_num = num;
  ASSERT_EQ(num, tmp_num);

  auto _ = num.checked_neg();
  ASSERT_EQ(num, tmp_num);
}

TEST(UintegerTest, UintegerOverflowingNeg) {
  for (uint32_t i = 10; i <= 1000; i += 10) {
    u32 num = i;
    auto [ret, flag] = num.overflowing_neg();
    ASSERT_TRUE(flag);
    ASSERT_EQ(ret, -i);
  }

  u32 max_num = u32::MAX;
  auto [ret, flag] = max_num.overflowing_neg();
  ASSERT_TRUE(flag);
  ASSERT_EQ(ret, -std::numeric_limits<uint32_t>::max());
  u32 min_num = u32::MIN;
  std::tie(ret, flag) = min_num.overflowing_neg();
  ASSERT_FALSE(flag);
  ASSERT_EQ(ret, min_num);
}

TEST(UintegerTest, UintegerOverflowingNegNoSideEffects) {
  u32 num = 10;
  u32 tmp_num = num;
  ASSERT_EQ(num, tmp_num);

  auto _ = num.overflowing_neg();
  ASSERT_EQ(num, tmp_num);
}

TEST(UintegerTest, UintegerWrappingNeg) {
  for (uint32_t i = 10; i <= 1000; i += 10) {
    u32 num = i;
    ASSERT_EQ(num.wrapping_neg(), -i);
  }

  u32 max_num = u32::MAX;
  ASSERT_EQ(max_num.wrapping_neg(), -std::numeric_limits<uint32_t>::max());
  u32 min_num = u32::MIN;
  ASSERT_EQ(min_num.wrapping_neg(), min_num);
}

TEST(UintegerTest, UintegerWrappingNegNoSideEffects) {
  u32 num = 10;
  u32 tmp_num = num;
  ASSERT_EQ(num, tmp_num);

  auto _ = num.wrapping_neg();
  ASSERT_EQ(num, tmp_num);
}
