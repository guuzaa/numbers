#include <numeric>

#include "gtest/gtest.h"

#include "numbers.h"
#include "test/utils.hh"

TEST(UintegerTest, UintegerTemplate) {
  Uinteger<unsigned long> test = 0;
  Uinteger<unsigned short> test1 = 0;
  Uinteger<unsigned long long> test2 = 0;
  Uinteger<uint8_t> test3 = 0;
  Uinteger<uint16_t> test4 = 0;
  Uinteger<uint32_t> test5 = 0;
  Uinteger<uint64_t> test6 = 0;
  Uinteger<unsigned int> test7 = 0;
  Uinteger<unsigned char> test8 = 0;
}

TEST(UintegerTest, UintegerStringStream) {
  for (unsigned short n = 0; n <= 1000; n++) {
    Uint16 num = n;
    std::ostringstream stream;
    stream << num;
    ASSERT_EQ(stream.str(), std::to_string(n));
  }

  for (unsigned long long n = 0; n < 1000; n++) {
    Uint64 num = n;
    std::ostringstream stream;
    stream << num;
    ASSERT_EQ(stream.str(), std::to_string(n));
  }
}

TEST(UintegerTest, UintegerAdd) {
  Uint32 num = 10_u32;
  Uint32 num1 = 10_u32;
  ASSERT_EQ(num, num1);

  Uint32 num2 = 20_u32;
  Uint32 zero_num = 0_u32;
  Uint32 max_num = Uint32::MAX;
  Uint32 min_num = Uint32::MIN;

  ASSERT_EQ(zero_num, min_num);
  ASSERT_EQ(num + num1, num2);
  ASSERT_EQ(zero_num + max_num, max_num);
  ASSERT_EQ(max_num + min_num, max_num);
}

TEST(UintegerTest, UintegerAddNoSideEffects) {
  Uint32 num = 10_u32;
  Uint32 num1 = num;
  Uint32 num2 = num;
  ASSERT_EQ(num1, num);
  ASSERT_EQ(num2, num);

  ASSERT_EQ(num2 + num1, 20_u32);
  ASSERT_EQ(num1, num);
  ASSERT_EQ(num2, num);
}

TEST(UintegerTest, UintegerWrappingAdd) {
  for (uint16_t n = std::numeric_limits<uint16_t>::min(); n < std::numeric_limits<uint16_t>::max(); n++) {
    Uint16 num = n;
    ASSERT_EQ(num.wrapping_add(1), Uint16(n + 1));
  }
  Uint16 num = Uint16::MAX;
  ASSERT_EQ(num.wrapping_add(1), Uint16(Uint16::MIN));
}

TEST(UintegerTest, UintegerWrappingAddNoSideEffects) {
  Uint32 num = 10_u32;
  Uint32 num1 = num;
  Uint32 num2 = num;
  ASSERT_EQ(num1, num);
  ASSERT_EQ(num2, num);

  ASSERT_EQ(num2.wrapping_add(num1), 20_u32);
  ASSERT_EQ(num1.wrapping_add(num2), 20_u32);
  ASSERT_EQ(num1, num);
  ASSERT_EQ(num2, num);
}

TEST(UintegerTest, UintegerAddByAccumulate) {
  constexpr size_t len = 100;
  std::vector<Uint64> nums(len);
  ASSERT_EQ(nums.size(), len);
  std::iota(nums.begin(), nums.end(), Uint64(1));
  ASSERT_EQ(std::accumulate(nums.begin(), nums.end(), Uint64(0)), Uint64(5050));
}

TEST(UintegerTest, UintegerCheckedAdd) {
  constexpr uint32_t min = 10;
  constexpr uint32_t max = 90000;
  for (int run = 0; run < 100; run++) {
    uint32_t a = random_integer(min, max);
    uint32_t b = random_integer(min, max);
    Uint32 num_a = a;
    Uint32 num_b = b;
    ASSERT_EQ(num_a.checked_add(num_b).value(), a + b);
  }

  {
    auto num = 10_u16;
    Uint16 num1 = Uint16::MAX;
    ASSERT_EQ(num.checked_add(num1), std::nullopt);
  }

  {
    auto num = 10_u64;
    Uint64 num1 = Uint64::MAX;
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
  Uint32 num = 10_u32;
  Uint32 num1 = num;
  Uint32 num2 = num;
  ASSERT_EQ(num1, num);
  ASSERT_EQ(num2, num);

  ASSERT_EQ(num2.checked_add(num1).value(), 20_u32);
  ASSERT_EQ(num1.checked_add(num2).value(), 20_u32);
  ASSERT_EQ(num1, num);
  ASSERT_EQ(num2, num);
}

TEST(UintegerTest, UintegerOverflowingAdd) {
  constexpr uint32_t min = 10;
  constexpr uint32_t max = 90000;
  for (int run = 0; run < 100; run++) {
    uint32_t a = random_integer(min, max);
    uint32_t b = random_integer(min, max);
    Uint32 num_a = a;
    Uint32 num_b = b;
    auto [ret, flag] = num_a.overflowing_add(num_b);
    ASSERT_FALSE(flag);
    ASSERT_EQ(ret, a + b);
  }

  {
    auto num = 10_u16;
    Uinteger<unsigned short> num1 = Uinteger<unsigned short>::MAX;
    auto [ret, flag] = num.overflowing_add(num1);
    ASSERT_TRUE(flag);
    ASSERT_EQ(ret, 10 + Uinteger<unsigned short>::MAX);
  }

  {
    auto num = 10_u64;
    Uint64 num1 = Uint64::MAX;
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
  Uint32 num = 10_u32;
  Uint32 num1 = num;
  Uint32 num2 = num;
  ASSERT_EQ(num1, num);
  ASSERT_EQ(num2, num);

  auto [ret, flag] = num2.overflowing_add(num1);
  ASSERT_EQ(ret, 20_u32);
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
    Uint32 num_a = a;
    Uint32 num_b = b;
    auto ret = num_a.saturating_add(num_b);
    ASSERT_EQ(ret, a + b);
  }

  for (uint8_t i = 1; i < std::numeric_limits<uint8_t>::max(); i++) {
    Uint8 num = Uint8::MAX;
    auto ret = num.saturating_add(Uint8(i));
    ASSERT_EQ(ret, Uint8(Uint8::MAX));
  }

  {
    Uint64 max_num = Uint64::MAX;
    auto ret = max_num.saturating_add(max_num);
    ASSERT_EQ(ret, Uint64::MAX);

    Uint64 min_num = Uint64::MIN;
    ret = min_num.saturating_add(min_num);
    ASSERT_EQ(ret, Uint64::MIN);
  }
}

TEST(UintegerTest, UintegerSaturatingAddNoSideEffects) {
  Uint32 num = 10_u32;
  Uint32 num1 = num;
  Uint32 num2 = num;
  ASSERT_EQ(num1, num);
  ASSERT_EQ(num2, num);

  auto ret = num2.saturating_add(num1);
  ASSERT_EQ(ret, 20_u32);
  ASSERT_EQ(num1, num);
  ASSERT_EQ(num2, num);
}

TEST(UintegerTest, UintegerAddOverflow) {
  {
    auto num = 10_u16;
    Uint16 num1 = Uint16::MAX;
    ASSERT_THROW(num + num1, std::runtime_error);
  }

  {
    auto num = 10_u64;
    Uint64 num1 = Uint64::MAX;
    ASSERT_THROW(num + num1, std::runtime_error);
  }
}

TEST(UintegerTest, UintegerAddOverflowByAccumulate) {
  std::vector<Uint64> nums(10);
  ASSERT_EQ(nums.size(), 10);
  std::iota(nums.begin(), nums.end(), Uint64(Uint64::MAX - 11));
  ASSERT_THROW(std::accumulate(nums.begin(), nums.end(), 0_u64), std::runtime_error);
}

TEST(UintegerTest, UintegerSub) {
  auto num = 10_u32;
  auto num1 = 10_u32;
  auto num2 = 20_u32;
  Uint32 num3 = Uint32::MAX;
  auto num4 = 0_u32;

  ASSERT_EQ(num2 - num1, num);
  ASSERT_EQ(num - num1, num4);
  ASSERT_EQ(num3 - num4, num3);

  constexpr uint32_t min = 10;
  constexpr uint32_t max = 90000;

  for (int run = 0; run < 100; run++) {
    uint32_t a = random_integer(min, max) + max;
    uint32_t b = random_integer(min, max);
    Uint32 num_a = a;
    Uint32 num_b = b;
    ASSERT_EQ(Uint32(a - b), num_a - num_b);
    ASSERT_EQ(num_a - num_b, a - b);
  }
}

TEST(UintegerTest, UintegerSubNoSideEffects) {
  Uint32 num = 10_u32;
  Uint32 num1 = num;
  Uint32 num2 = num;
  ASSERT_EQ(num1, num);
  ASSERT_EQ(num2, num);

  ASSERT_EQ(num2 - num1, 0_u32);
  ASSERT_EQ(num1, num);
  ASSERT_EQ(num2, num);
}

TEST(UintegerTest, UintegerSubOverflow) {
  for (short n = 1; n < 1000; n++) {
    Uint16 num = n;
    Uint16 num1 = Uint16::MIN;
    ASSERT_THROW(num1 - num, std::runtime_error);
  }
}

TEST(UintegerTest, UintegerCheckedSub) {
  for (short n = 1; n < 1000; n++) {
    Uint16 num = n;
    Uint16 num1 = Uint16::MIN;
    ASSERT_EQ(num1.checked_sub(num), std::nullopt);
  }

  for (uint16_t n = std::numeric_limits<uint16_t>::max(); n > std::numeric_limits<uint16_t>::min(); n--) {
    Uint16 num = n;
    auto ret = num.checked_sub(1);
    ASSERT_EQ(ret.value(), Uint16(n - 1));
  }
}

TEST(UintegerTest, UintegerCheckedSubNoSideEffects) {
  Uint32 num = 10_u32;
  Uint32 num1 = num;
  Uint32 num2 = num;
  ASSERT_EQ(num1, num);
  ASSERT_EQ(num2, num);

  ASSERT_EQ(num2.checked_sub(num1).value(), 0_u32);
  ASSERT_EQ(num1, num);
  ASSERT_EQ(num2, num);
}

TEST(UintegerTest, UintegerOverflowingSub) {
  for (unsigned short n = 1; n < 1000; n++) {
    Uint16 num = n;
    Uint16 num1 = Uint16::MIN;
    auto [ret, flag] = num1.overflowing_sub(num);
    ASSERT_TRUE(flag);
    ASSERT_EQ(ret, Uint16::MIN - n);
  }

  for (uint16_t n = std::numeric_limits<uint16_t>::max(); n > std::numeric_limits<uint16_t>::min(); n--) {
    Uint16 num = n;
    auto [ret, flag] = num.overflowing_sub(1);
    ASSERT_FALSE(flag);
    ASSERT_EQ(ret, Uint16(n - 1));
  }
}

TEST(UintegerTest, UintegerOverflowingSubNoSideEffects) {
  Uint32 num = 10_u32;
  Uint32 num1 = num;
  Uint32 num2 = num;
  ASSERT_EQ(num1, num);
  ASSERT_EQ(num2, num);

  auto [ret, flag] = num2.overflowing_sub(num1);
  ASSERT_EQ(ret, 0_u32);
  ASSERT_FALSE(flag);
  ASSERT_EQ(num1, num);
  ASSERT_EQ(num2, num);
}

TEST(UintegerTest, UintegerWrappingSub) {
  for (uint16_t n = std::numeric_limits<uint16_t>::max(); n > std::numeric_limits<uint16_t>::min(); n--) {
    Uint16 num = n;
    ASSERT_EQ(num.wrapping_sub(1), Uint16(n - 1));
  }

  Uint16 num = Uint16::MIN;
  ASSERT_EQ(num.wrapping_sub(1), Uint16(Uint16::MAX));
}

TEST(UintegerTest, UintegerWarppingSubNoSideEffects) {
  Uint32 num = 10_u32;
  Uint32 num1 = num;
  Uint32 num2 = num;
  ASSERT_EQ(num1, num);
  ASSERT_EQ(num2, num);

  auto ret = num2.wrapping_sub(num1);
  ASSERT_EQ(ret, 0_u32);
  ASSERT_EQ(num1, num);
  ASSERT_EQ(num2, num);
}

TEST(UintegerTest, UintegerSaturatingSub) {
  for (unsigned short n = 1; n <= 1000; n++) {
    Uint16 num = n;
    Uint16 num1 = Uint16::MIN;
    auto ret = num1.saturating_sub(num);
    ASSERT_EQ(ret, Uint16::MIN);
  }

  for (uint16_t n = std::numeric_limits<uint16_t>::max(); n > std::numeric_limits<uint16_t>::min(); n--) {
    Uint16 num = n;
    auto ret = num.saturating_sub(1);
    ASSERT_EQ(ret, Uint16(n - 1));
  }

  Uint16 max_num = Uint16::MAX;
  Uint16 min_num = Uint16::MIN;
  auto ret = max_num.saturating_sub(min_num);
  ASSERT_EQ(ret, Uint16::MAX);
  ret = min_num.saturating_sub(max_num);
  ASSERT_EQ(ret, Uint16::MIN);
}

TEST(UintegerTest, UintegerSaturatingSubNoSideEffects) {
  Uint32 num = 10_u32;
  Uint32 num1 = num;
  Uint32 num2 = num;
  ASSERT_EQ(num1, num);
  ASSERT_EQ(num2, num);

  auto ret = num2.saturating_sub(num1);
  ASSERT_EQ(ret, 0_u32);
  ASSERT_EQ(num1, num);
  ASSERT_EQ(num2, num);
}

TEST(UintegerTest, UintegerMul) {
  auto num = 10_u64;
  auto num1 = 100_u64;
  ASSERT_EQ(num1 * num, 1000);
  num = num.MAX;
  num1 = 0_u64;
  ASSERT_EQ(num1 * num, 0);
  num1 = 1_u64;
  ASSERT_EQ(num1 * num, num);

  constexpr uint32_t min = 10;
  constexpr uint32_t max = 10000;
  for (int run = 0; run < 100; run++) {
    uint32_t a = random_integer(min, max);
    uint32_t b = random_integer(min, max);
    Uint32 num_a = a;
    Uint32 num_b = b;
    ASSERT_EQ(Uint32(a * b), num_a * num_b);
    ASSERT_EQ(num_a * num_b, a * b);
  }
}

TEST(UintegerTest, UintegerMulNoSideEffects) {
  Uint32 num = 10_u32;
  Uint32 num1 = num;
  Uint32 num2 = num;
  ASSERT_EQ(num1, num);
  ASSERT_EQ(num2, num);

  auto ret = num2 * num1;
  ASSERT_EQ(ret, 100_u32);
  ASSERT_EQ(num1, num);
  ASSERT_EQ(num2, num);
}

TEST(UintegerTest, UintegerCheckedMul) {
  auto num = 10_u8;
  auto num1 = 100_u8;
  ASSERT_EQ(num1.checked_mul(num), std::nullopt);

  constexpr uint32_t min = 10;
  constexpr uint32_t max = 10000;
  for (int run = 0; run < 100; run++) {
    uint32_t a = random_integer(min, max);
    uint32_t b = random_integer(min, max);
    Uint32 num_a = a;
    Uint32 num_b = b;
    ASSERT_EQ(num_a.checked_mul(num_b).value(), a * b);
  }
}

TEST(UintegerTest, UintegerCheckedMulNoSideEffects) {
  Uint32 num = 10_u32;
  Uint32 num1 = num;
  Uint32 num2 = num;
  ASSERT_EQ(num1, num);
  ASSERT_EQ(num2, num);

  auto ret = num2.checked_mul(num1);
  ASSERT_EQ(ret.value(), 100_u32);
  ASSERT_EQ(num1, num);
  ASSERT_EQ(num2, num);
}

TEST(UintegerTest, UintegerOverflowingMul) {
  auto num = 10_u8;
  auto num1 = 100_u8;
  auto [ret, flag] = num1.overflowing_mul(num);
  ASSERT_EQ(ret, uint8_t(10 * 100));
  ASSERT_TRUE(flag);

  constexpr uint32_t min = 10;
  constexpr uint32_t max = 10000;
  for (int run = 0; run < 100; run++) {
    uint32_t a = random_integer(min, max);
    uint32_t b = random_integer(min, max);
    Uint32 num_a = a;
    Uint32 num_b = b;
    auto [ret, flag] = num_a.overflowing_mul(num_b);
    ASSERT_FALSE(flag);
    ASSERT_EQ(ret, a * b);
  }
}

TEST(UntegerTest, UintegerOverflowingMulNoSideEffects) {
  Uint32 num = 10_u32;
  Uint32 num1 = num;
  Uint32 num2 = num;
  ASSERT_EQ(num1, num);
  ASSERT_EQ(num2, num);

  auto [ret, flag] = num2.overflowing_mul(num1);
  ASSERT_FALSE(flag);
  ASSERT_EQ(ret, 100_u32);
  ASSERT_EQ(num1, num);
  ASSERT_EQ(num2, num);
}

TEST(UintegerTest, UintegerSaturatingMul) {
  auto num = 10_u8;
  auto num1 = 100_u8;
  auto ret = num1.saturating_mul(num);
  ASSERT_EQ(ret, Uint8::MAX);

  Uint16 max_num = Uint16::MAX;
  ASSERT_EQ(max_num.saturating_mul(max_num), Uint16::MAX);
  Uint16 min_num = Uint16::MIN;
  ASSERT_EQ(min_num.saturating_mul(min_num), min_num);

  constexpr uint32_t min = 10;
  constexpr uint32_t max = 10000;
  for (int run = 0; run < 100; run++) {
    uint32_t a = random_integer(min, max);
    uint32_t b = random_integer(min, max);
    Uint32 num_a = a;
    Uint32 num_b = b;
    auto ret = num_a.saturating_mul(num_b);
    ASSERT_EQ(ret, a * b);
  }
}

TEST(UintegerTest, UintegerSaturatingMulNoSideEffects) {
  Uint32 num = 10_u32;
  Uint32 num1 = num;
  Uint32 num2 = num;
  ASSERT_EQ(num1, num);
  ASSERT_EQ(num2, num);

  auto ret = num2.saturating_mul(num1);
  ASSERT_EQ(ret, 100_u32);
  ASSERT_EQ(num1, num);
  ASSERT_EQ(num2, num);
}

TEST(UintegerTest, UintegerDiv) {
  constexpr uint32_t min = 10;
  constexpr uint32_t max = 10000;
  for (int run = 0; run < 100; run++) {
    uint32_t a = random_integer(min, max);
    uint32_t b = random_integer(min, max);
    Uint32 num_a = a;
    Uint32 num_b = b;
    ASSERT_EQ(Uint32(a / b), num_a / num_b);
    ASSERT_EQ(num_a / num_b, a / b);
  }
}

TEST(UintegerTest, UintegerDivNoSideEffects) {
  Uint32 num = 10_u32;
  Uint32 num1 = num;
  Uint32 num2 = num;
  ASSERT_EQ(num1, num);
  ASSERT_EQ(num2, num);

  auto ret = num2 / num1;
  ASSERT_EQ(ret, 1_u32);
  ASSERT_EQ(num1, num);
  ASSERT_EQ(num2, num);
}

TEST(UintegerTest, UintegerWrappingDiv) {
  Uint8 num = 10_u8;
  Uint8 num1 = Uint8::MIN;
  ASSERT_EQ(num1.wrapping_div(num), num1);
}

TEST(UintegerTest, UintegerWrappingDivNoSideEffects) {
  Uint32 num = 10_u32;
  Uint32 num1 = num;
  Uint32 num2 = num;
  ASSERT_EQ(num1, num);
  ASSERT_EQ(num2, num);

  auto ret = num2.wrapping_div(num1);
  ASSERT_EQ(ret, Uint32(1));
  ASSERT_EQ(num1, num);
  ASSERT_EQ(num2, num);
}

TEST(UintegerTest, UintegerCheckedDiv) {
  constexpr uint32_t min = 10;
  constexpr uint32_t max = 10000;
  for (int run = 0; run < 100; run++) {
    uint32_t a = random_integer(min, max);
    uint32_t b = random_integer(min, max);
    Uint32 num_a = a;
    Uint32 num_b = b;
    EXPECT_EQ(num_a.checked_div(num_b).value(), a / b);
  }
}

TEST(UintegerTest, UintegerCheckedDivNoSideEffects) {
  Uint32 num = 10_u32;
  Uint32 num1 = num;
  Uint32 num2 = num;
  ASSERT_EQ(num1, num);
  ASSERT_EQ(num2, num);

  auto ret = num2.checked_div(num1);
  ASSERT_EQ(ret.value(), 1_u32);
  ASSERT_EQ(num1, num);
  ASSERT_EQ(num2, num);
}

TEST(UintegerTest, UintegerOverflowingDiv) {
  constexpr uint32_t min = 10;
  constexpr uint32_t max = 10000;
  for (int run = 0; run < 100; run++) {
    uint32_t a = random_integer(min, max);
    uint32_t b = random_integer(min, max);
    Uint32 num_a = a;
    Uint32 num_b = b;
    auto [ret, flag] = num_a.overflowing_div(num_b);
    ASSERT_FALSE(flag);
    EXPECT_EQ(ret, a / b);
  }
}

TEST(UintegerTest, UintegerOverflowingDivNoSideEffects) {
  Uint32 num = 10_u32;
  Uint32 num1 = num;
  Uint32 num2 = num;
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
    Uint32 num_a = a;
    Uint32 num_b = b;
    auto ret = num_a.saturating_div(num_b);
    EXPECT_EQ(ret, a / b);
  }
}

TEST(UintegerTest, UintegerSaturatingDivNoSideEffects) {
  Uint32 num = 10_u32;
  Uint32 num1 = num;
  Uint32 num2 = num;
  ASSERT_EQ(num1, num);
  ASSERT_EQ(num2, num);

  auto ret = num2.saturating_div(num1);
  ASSERT_EQ(num1, num);
  ASSERT_EQ(num2, num);
}

TEST(UintegerTest, UintegerNeg) {
  Uint32 num = 0;
  ASSERT_EQ(-num, num);

  for (uint32_t i = 1; i <= 1000; i += 10) {
    num = i;
    ASSERT_THROW(-num, std::runtime_error);
  }
}

TEST(UintegerTest, UintegerNegNoSideEffects) {
  Uint32 num = 10_u32;
  Uint32 tmp_num = num;
  ASSERT_EQ(num, tmp_num);

  ASSERT_THROW(-num, std::runtime_error);
  ASSERT_EQ(num, tmp_num);
}

TEST(UintegerTest, UintegerCheckedNeg) {
  for (uint32_t i = 1; i <= 1000; i += 10) {
    Uint32 num = i;
    ASSERT_EQ(num.checked_neg(), std::nullopt);
  }

  Int32 min_num = Uint32::MIN;
  ASSERT_EQ(min_num.checked_neg(), min_num);
}

TEST(UintegerTest, UintegerCheckedNegNoSideEffects) {
  Uint32 num = 10_u32;
  Uint32 tmp_num = num;
  ASSERT_EQ(num, tmp_num);

  auto _ = num.checked_neg();
  ASSERT_EQ(num, tmp_num);
}

TEST(UintegerTest, UintegerOverflowingNeg) {
  for (uint32_t i = 10; i <= 1000; i += 10) {
    Uint32 num = i;
    auto [ret, flag] = num.overflowing_neg();
    ASSERT_TRUE(flag);
    ASSERT_EQ(ret, -i);
  }

  Uint32 max_num = Uint32::MAX;
  auto [ret, flag] = max_num.overflowing_neg();
  ASSERT_TRUE(flag);
  ASSERT_EQ(ret, -Uint32::MAX);
  Uint32 min_num = Uint32::MIN;
  std::tie(ret, flag) = min_num.overflowing_neg();
  ASSERT_FALSE(flag);
  ASSERT_EQ(ret, min_num);
}

TEST(UintegerTest, UintegerOverflowingNegNoSideEffects) {
  Uint32 num = 10_u32;
  Uint32 tmp_num = num;
  ASSERT_EQ(num, tmp_num);

  auto _ = num.overflowing_neg();
  ASSERT_EQ(num, tmp_num);
}

TEST(UintegerTest, UintegerWrappingNeg) {
  for (uint32_t i = 10; i <= 1000; i += 10) {
    Uint32 num = i;
    ASSERT_EQ(num.wrapping_neg(), -i);
  }

  Uint32 max_num = Uint32::MAX;
  ASSERT_EQ(max_num.wrapping_neg(), -Uint32::MAX);
  Uint32 min_num = Uint32::MIN;
  ASSERT_EQ(min_num.wrapping_neg(), min_num);
}

TEST(UintegerTest, UintegerWrappingNegNoSideEffects) {
  Uint32 num = 10_u32;
  Uint32 tmp_num = num;
  ASSERT_EQ(num, tmp_num);

  auto _ = num.wrapping_neg();
  ASSERT_EQ(num, tmp_num);
}
