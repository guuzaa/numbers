#include <numeric>
#include <unordered_set>
#include <vector>
#include "gtest/gtest.h"

#include "integer.hh"
#include "test/utils.hh"

using namespace numbers;

typedef ::testing::Types<bool, char, signed char, unsigned char, char16_t, char32_t, short, unsigned short, int,
                         unsigned int, long, unsigned long, long long, unsigned long long>
    IntegerTypes;

template <typename T>
class IntegerTraitsTest : public ::testing::Test {};

TYPED_TEST_SUITE(IntegerTraitsTest, IntegerTypes);

TYPED_TEST(IntegerTraitsTest, ConstructAssignTest) {
  static_assert(std::is_constructible<i8, TypeParam>::value, "numbers::i8 must be constructible from TypeParam");
  static_assert(std::is_assignable<i8 &, TypeParam>::value, "numbers::i8 must be assignable from TypeParam");
  static_assert(!std::is_assignable<TypeParam &, i8>::value, "TypeParam must not be assignable from numbers::i8");

  static_assert(std::is_constructible<i16, TypeParam>::value, "numbers::i16 must be constructible from TypeParam");
  static_assert(std::is_assignable<i16 &, TypeParam>::value, "numbers::i16 must be assignable from TypeParam");
  static_assert(!std::is_assignable<TypeParam &, i16>::value, "TypeParam must not be assignable from numbers::i16");

  static_assert(std::is_constructible<i32, TypeParam>::value, "numbers::i32 must be constructible from TypeParam");
  static_assert(std::is_assignable<i32 &, TypeParam>::value, "numbers::i32 must be assignable from TypeParam");
  static_assert(!std::is_assignable<TypeParam &, i32>::value, "TypeParam must not be assignable from numbers::i32");

  static_assert(std::is_constructible<i64, TypeParam>::value, "numbers::i64 must be constructible from TypeParam");
  static_assert(std::is_assignable<i64 &, TypeParam>::value, "numbers::i64 must be assignable from TypeParam");
  static_assert(!std::is_assignable<TypeParam &, i64>::value, "TypeParam must not be assignable from numbers::i64");
}

typedef ::testing::Types<i8, i16, i32, i64, i128> Integers;

template <typename T>
class integerTest : public ::testing::Test {};

TYPED_TEST_SUITE(integerTest, Integers);

TYPED_TEST(integerTest, BoolConversion) {
  EXPECT_FALSE(TypeParam(0));
  for (int i = 0; i < sizeof(TypeParam); ++i) {
    EXPECT_TRUE(TypeParam(1) << i);
  }

  EXPECT_TRUE(TypeParam::MIN);
  EXPECT_TRUE(TypeParam::MAX);
  EXPECT_EQ(TypeParam(1), TypeParam(true));
  EXPECT_EQ(TypeParam(0), TypeParam(false));
}

TYPED_TEST(integerTest, IncrementDecrement) {
  TypeParam val = 0;
  EXPECT_EQ(0, val++);
  EXPECT_EQ(1, val);
  EXPECT_EQ(1, val--);
  EXPECT_EQ(0, val);
  EXPECT_EQ(-1, --val);
  EXPECT_EQ(-1, val);
  EXPECT_EQ(0, ++val);
  EXPECT_EQ(0, val);
}

TEST(integerTest, IntegerMinMax) {
  ASSERT_THROW(-i64::MIN, std::runtime_error);
  ASSERT_EQ(i16::MIN.saturating_neg(), i16::MAX);
  ASSERT_EQ(i16::MAX.saturating_add(i16::MAX), i16::MAX);
  ASSERT_EQ(i16::MAX.saturating_sub(i16::MAX), 0);
}

TEST(integerTest, integerStringStream) {
  for (short n = 1; n < 1000; n++) {
    i16 num = n;
    std::ostringstream stream;
    stream << num;
    ASSERT_EQ(stream.str(), std::to_string(n));
  }

  for (long long n = 1; n < 1000; n++) {
    i64 num = n;
    std::ostringstream stream;
    stream << num;
    ASSERT_EQ(stream.str(), std::to_string(n));
  }
}

TEST(integerTest, integerAdd) {
  i32 num = 10;
  i32 num1 = 10;
  ASSERT_EQ(num, num1);

  i32 num2 = 20;
  i32 zero_num = 0;
  i32 max_num = i32::MAX;
  i32 min_num = i32::MIN;

  ASSERT_EQ(num + num1, num2);
  ASSERT_EQ(zero_num + max_num, max_num);
  ASSERT_EQ(max_num += min_num, -1);
}

TEST(integerTest, integerAddNoSideEffects) {
  i32 num = 10;
  i32 num1 = num;
  i32 num2 = num;
  ASSERT_EQ(num1, num);
  ASSERT_EQ(num2, num);

  ASSERT_EQ(num2 + num1, 20);
  ASSERT_EQ(num1, num);
  ASSERT_EQ(num2, num);
}

TEST(integerTest, integerWrappingAdd) {
  for (int16_t n = std::numeric_limits<int16_t>::min(); n < std::numeric_limits<int16_t>::max(); n++) {
    i16 num = n;
    ASSERT_EQ(num.wrapping_add(1), i16(n + 1));
  }
  i16 num = i16::MAX;
  ASSERT_EQ(num.wrapping_add(1), i16::MIN);

  constexpr int32_t min = 10;
  constexpr int32_t max = 90000;
  for (int run = 0; run < 100; run++) {
    int32_t a = random_integer(min, max);
    int32_t b = random_integer(min, max);
    i32 num_a = a;
    i32 num_b = b;
    ASSERT_EQ(i32(a + b), num_a + num_b);
    ASSERT_EQ(num_a + num_b, a + b);
  }
}

TEST(integerTest, integerWrappingAddNoSideEffects) {
  i32 num = 10;
  i32 num1 = num;
  i32 num2 = num;
  ASSERT_EQ(num1, num);
  ASSERT_EQ(num2, num);

  ASSERT_EQ(num2.wrapping_add(num1), i32(20));
  ASSERT_EQ(num1, num);
  ASSERT_EQ(num2, num);
}

TEST(integerTest, integerAddByAccumulate) {
  constexpr size_t len = 100;
  std::vector<i64> nums(len);
  ASSERT_EQ(nums.size(), len);
  std::iota(nums.begin(), nums.end(), 1);
  ASSERT_EQ(std::accumulate(nums.begin(), nums.end(), i64(0)), i64(5050));
}

TEST(integerTest, integerCheckedAdd) {
  constexpr int32_t min = 10;
  constexpr int32_t max = 90000;
  for (int run = 0; run < 100; run++) {
    int32_t a = random_integer(min, max);
    int32_t b = random_integer(min, max);
    i32 num_a = a;
    i32 num_b = b;
    ASSERT_EQ(num_a.checked_add(num_b).value(), a + b);
  }

  {
    i16 num = 10;
    i16 num1 = i16::MAX;
    ASSERT_EQ(num.checked_add(num1), std::nullopt);
  }

  {
    i64 num = 10;
    i64 num1 = i64::MAX;
    ASSERT_EQ(num.checked_add(num1), std::nullopt);
  }

  {
    i64 num = -1;
    i64 num1 = i64::MIN;
    ASSERT_EQ(num.checked_add(num1), std::nullopt);
  }

  {
    i64 num1 = i64::MIN;
    ASSERT_EQ(num1.checked_add(num1), std::nullopt);
    i64 num2 = i64::MAX;
    ASSERT_EQ(num2.checked_add(num2), std::nullopt);
  }
}

TEST(integerTest, integerCheckedAddNoSideEffects) {
  i32 num = 10;
  i32 num1 = num;
  i32 num2 = num;
  ASSERT_EQ(num1, num);
  ASSERT_EQ(num2, num);

  ASSERT_EQ(num2.checked_add(num1).value(), i32(20));
  ASSERT_EQ(num1, num);
  ASSERT_EQ(num2, num);
}

TEST(integerTest, integerOverflowingAdd) {
  constexpr int32_t min = 10;
  constexpr int32_t max = 90000;
  for (int run = 0; run < 100; run++) {
    int32_t a = random_integer(min, max);
    int32_t b = random_integer(min, max);
    i32 num_a = a;
    i32 num_b = b;
    auto [ret, flag] = num_a.overflowing_add(num_b);
    ASSERT_FALSE(flag);
    ASSERT_EQ(ret, a + b);
  }

  {
    i16 num = 10;
    i16 num1 = i16::MAX;
    auto [ret, flag] = num.overflowing_add(num1);
    ASSERT_TRUE(flag);
    ASSERT_EQ(ret, 10 + std::numeric_limits<int16_t>::max());
  }

  {
    i64 num = 10;
    i64 num1 = i64::MAX;
    auto [_, flag] = num.overflowing_add(num1);
    ASSERT_TRUE(flag);
  }

  {
    i64 num = -1;
    i64 num1 = i64::MIN;
    auto [_, flag] = num.overflowing_add(num1);
    ASSERT_TRUE(flag);
  }

  {
    i64 num1 = i64::MIN;
    auto [_, flag1] = num1.overflowing_add(num1);
    ASSERT_TRUE(flag1);
  }

  {
    i64 num2 = i64::MAX;
    auto [_, flag2] = num2.overflowing_add(num2);
    ASSERT_TRUE(flag2);
  }
}

TEST(integerTest, integerOverflowingAddNoSideEffects) {
  i32 num = 10;
  i32 num1 = num;
  i32 num2 = num;
  ASSERT_EQ(num1, num);
  ASSERT_EQ(num2, num);

  auto [ret, flag] = num2.overflowing_add(num1);
  ASSERT_EQ(ret, i32(20));
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
    i32 num_a = a;
    i32 num_b = b;
    auto ret = num_a.saturating_add(num_b);
    ASSERT_EQ(ret, a + b);
  }

  for (int8_t i = 1; i < std::numeric_limits<int8_t>::max(); i++) {
    i8 num = i8::MAX;
    auto ret = num.saturating_add(i8(i));
    EXPECT_EQ(ret, i8::MAX) << static_cast<int>(i) << '\n';
  }

  {
    i16 num = 10;
    i16 num1 = i16::MAX;
    auto ret = num.saturating_add(num1);
    ASSERT_EQ(ret, i16::MAX);
  }

  {
    i64 num = 10;
    i64 num1 = i64::MAX;
    auto ret = num.saturating_add(num1);
    ASSERT_EQ(ret, i64::MAX);
  }

  {
    i64 max_num = i64::MAX;
    auto ret = max_num.saturating_add(max_num);
    ASSERT_EQ(ret, i64::MAX);

    i64 min_num = i64::MIN;
    ret = min_num.saturating_add(min_num);
    ASSERT_EQ(ret, i64::MIN);
  }

  {
    i64 num = -1;
    i64 num1 = i64::MIN;
    auto ret = num.saturating_add(num1);
    ASSERT_EQ(ret, i64::MIN);
  }
}

TEST(integerTest, integerSaturatingAddNoSideEffects) {
  i32 num = 10;
  i32 num1 = num;
  i32 num2 = num;
  ASSERT_EQ(num1, num);
  ASSERT_EQ(num2, num);

  auto ret = num2.saturating_add(num1);
  ASSERT_EQ(ret, i32(20));
  ASSERT_EQ(num1, num);
  ASSERT_EQ(num2, num);
}

TEST(integerTest, integerAddOverflow) {
  {
    i16 num = 10;
    i16 num1 = i16::MAX;
    ASSERT_THROW(num + num1, std::runtime_error);
  }

  {
    i64 num = 10;
    i64 num1 = i64::MAX;
    ASSERT_THROW(num + num1, std::runtime_error);
  }

  {
    i64 num = -1;
    i64 num1 = i64::MIN;
    ASSERT_THROW(num + num1, std::runtime_error);
  }

  {
    i64 num = i64::MAX;
    ASSERT_THROW(++num, std::runtime_error);
  }

  {
    i64 num = i64::MAX;
    ASSERT_THROW(num++, std::runtime_error);
  }
}

TEST(integerTest, integerAddOverflowByAccumulate) {
  std::vector<i64> nums(10);
  ASSERT_EQ(nums.size(), 10);
  std::iota(nums.begin(), nums.end(), i64::MAX - 11);
  ASSERT_THROW(std::accumulate(nums.begin(), nums.end(), i64(0)), std::runtime_error);
}

TEST(integerTest, integerSub) {
  auto num = i32(10);
  auto num1 = i32(10);
  auto num2 = i32(20);
  i32 num3 = i32::MAX;
  auto num4 = i32(0);

  ASSERT_EQ(num2 - num1, num);
  ASSERT_EQ(num - num1, num4);
  ASSERT_EQ(num3 - num4, num3);

  {
    i64 num = -1;
    i64 num1 = i64::MIN;
    ASSERT_EQ(num - num1, i64::MAX);
  }

  constexpr int32_t min = 10;
  constexpr int32_t max = 90000;

  for (int run = 0; run < 100; run++) {
    int32_t a = random_integer(min, max);
    int32_t b = random_integer(min, max);
    i32 num_a = a;
    i32 num_b = b;
    ASSERT_EQ(i32(a - b), num_a - num_b);
    ASSERT_EQ(num_a - num_b, a - b);
  }
}

TEST(integerTest, integerSubNoSideEffects) {
  i32 num = 10;
  i32 num1 = num;
  i32 num2 = num;
  ASSERT_EQ(num1, num);
  ASSERT_EQ(num2, num);

  ASSERT_EQ(num2 - num1, i32(0));
  ASSERT_EQ(num1, num);
  ASSERT_EQ(num2, num);
}

TEST(integerTest, integerSubOverflow) {
  for (short n = 1; n < 1000; n++) {
    i16 num = n;
    i16 num1 = i16::MIN;
    ASSERT_THROW(num1 - num, std::runtime_error);
  }
  {
    i16 num1 = i16::MIN;
    ASSERT_THROW(num1--, std::runtime_error);
  }
  {
    i16 num1 = i16::MIN;
    ASSERT_THROW(--num1, std::runtime_error);
  }
  {
    i16 num1 = i16::MIN;
    ASSERT_THROW(num1 -= 1, std::runtime_error);
  }
}

TEST(integerTest, integerCheckedSub) {
  for (short n = 1; n < 1000; n++) {
    i16 num = n;
    i16 num1 = i16::MIN;
    ASSERT_EQ(num1.checked_sub(num), std::nullopt);
  }

  for (int16_t n = std::numeric_limits<int16_t>::max(); n > std::numeric_limits<int16_t>::min(); n--) {
    i16 num = n;
    auto ret = num.checked_sub(1);
    ASSERT_EQ(ret.value(), i16(n - 1));
  }
}

TEST(integerTest, integerCheckedSubNoSideEffects) {
  i32 num = 10;
  i32 num1 = num;
  i32 num2 = num;
  ASSERT_EQ(num1, num);
  ASSERT_EQ(num2, num);

  ASSERT_EQ(num2.checked_sub(num1).value(), i32(0));
  ASSERT_EQ(num1, num);
  ASSERT_EQ(num2, num);
}

TEST(integerTest, integerOverflowingSub) {
  for (short n = 1; n < 1000; n++) {
    i16 num = n;
    i16 num1 = i16::MIN;
    auto [ret, flag] = num1.overflowing_sub(num);
    ASSERT_TRUE(flag);
    ASSERT_EQ(ret, std::numeric_limits<int16_t>::min() - n);
  }

  for (int16_t n = std::numeric_limits<int16_t>::max(); n > std::numeric_limits<int16_t>::min(); n--) {
    i16 num = n;
    auto [ret, flag] = num.overflowing_sub(1);
    ASSERT_FALSE(flag);
    ASSERT_EQ(ret, i16(n - 1));
  }
}

TEST(integerTest, integerOverflowingSubNoSideEffects) {
  i32 num = 10;
  i32 num1 = num;
  i32 num2 = num;
  ASSERT_EQ(num1, num);
  ASSERT_EQ(num2, num);

  auto [ret, flag] = num2.overflowing_sub(num1);
  ASSERT_EQ(ret, i32(0));
  ASSERT_FALSE(flag);
  ASSERT_EQ(num1, num);
  ASSERT_EQ(num2, num);
}

TEST(integerTest, integerWrappingSub) {
  for (int16_t n = std::numeric_limits<int16_t>::max(); n > std::numeric_limits<int16_t>::min(); n--) {
    i16 num = n;
    ASSERT_EQ(num.wrapping_sub(1), i16(n - 1));
  }
  i16 num = i16::MIN;
  ASSERT_EQ(num.wrapping_sub(1), i16::MAX);
}

TEST(integerTest, integerWarppingSubNoSideEffects) {
  i32 num = 10;
  i32 num1 = num;
  i32 num2 = num;
  ASSERT_EQ(num1, num);
  ASSERT_EQ(num2, num);

  auto ret = num2.wrapping_sub(num1);
  ASSERT_EQ(ret, i32(0));
  ASSERT_EQ(num1, num);
  ASSERT_EQ(num2, num);
}

TEST(integerTest, integerSaturatingSub) {
  for (short n = 1; n <= 1000; n++) {
    i16 num = n;
    i16 num1 = i16::MIN;
    auto ret = num1.saturating_sub(num);
    ASSERT_EQ(ret, i16::MIN);
  }

  for (int16_t n = std::numeric_limits<int16_t>::max(); n > std::numeric_limits<int16_t>::min(); n--) {
    i16 num = n;
    auto ret = num.saturating_sub(1);
    ASSERT_EQ(ret, i16(n - 1));
  }

  i16 min_num = i16::MIN;
  auto ret = min_num.saturating_sub(1);
  ASSERT_EQ(ret, i16::MIN);

  i16 max_num = i16::MAX;
  ret = max_num.saturating_sub(min_num);
  ASSERT_EQ(ret, i16::MAX);

  ret = min_num.saturating_sub(max_num);
  ASSERT_EQ(ret, i16::MIN);
}

TEST(integerTest, integerSaturatingSubNoSideEffects) {
  i32 num = 10;
  i32 num1 = num;
  i32 num2 = num;
  ASSERT_EQ(num1, num);
  ASSERT_EQ(num2, num);

  auto ret = num2.saturating_sub(num1);
  ASSERT_EQ(ret, i32(0));
  ASSERT_EQ(num1, num);
  ASSERT_EQ(num2, num);
}

TEST(integerTest, integerMul) {
  auto num = i64(10);
  auto num1 = i64(100);
  ASSERT_EQ(num1 * num, 1000);
  num = num.MAX;
  num1 = i64(0);
  ASSERT_EQ(num1 * num, 0);
  num1 = i64(1);
  ASSERT_EQ(num1 * num, num);

  constexpr int32_t min = 10;
  constexpr int32_t max = 10000;
  for (int run = 0; run < 100; run++) {
    int32_t a = random_integer(min, max);
    int32_t b = random_integer(min, max);
    i32 num_a = a;
    i32 num_b = b;
    ASSERT_EQ(i32(a * b), num_a * num_b);
    ASSERT_EQ(num_a * num_b, a * b);
  }
}

TEST(integerTest, integerMulNoSideEffects) {
  i32 num = 10;
  i32 num1 = num;
  i32 num2 = num;
  ASSERT_EQ(num1, num);
  ASSERT_EQ(num2, num);

  auto ret = num2 * num1;
  ASSERT_EQ(ret, i32(100));
  ASSERT_EQ(num1, num);
  ASSERT_EQ(num2, num);
}

TEST(integerTest, integerMulOverflow) {
  auto num = i8(10);
  auto num1 = i8(100);
  ASSERT_THROW(num1 * num, std::runtime_error);
}

TEST(integerTest, integerCheckedMul) {
  auto num = i8(10);
  auto num1 = i8(100);
  ASSERT_EQ(num1.checked_mul(num), std::nullopt);

  constexpr int32_t min = 10;
  constexpr int32_t max = 10000;
  for (int run = 0; run < 100; run++) {
    int32_t a = random_integer(min, max);
    int32_t b = random_integer(min, max);
    i32 num_a = a;
    i32 num_b = b;
    ASSERT_EQ(num_a.checked_mul(num_b).value(), a * b);
  }
}

TEST(integerTest, integerCheckedMulNoSideEffects) {
  i32 num = 10;
  i32 num1 = num;
  i32 num2 = num;
  ASSERT_EQ(num1, num);
  ASSERT_EQ(num2, num);

  auto ret = num2.checked_mul(num1);
  ASSERT_EQ(ret.value(), i32(100));
  ASSERT_EQ(num1, num);
  ASSERT_EQ(num2, num);
}

TEST(integerTest, integerOverflowingMul) {
  auto num = i8(10);
  auto num1 = i8(100);
  auto [ret, flag] = num1.overflowing_mul(num);
  ASSERT_EQ(ret, int8_t(10 * 100));
  ASSERT_TRUE(flag);

  constexpr int32_t min = 10;
  constexpr int32_t max = 10000;
  for (int run = 0; run < 100; run++) {
    int32_t a = random_integer(min, max);
    int32_t b = random_integer(min, max);
    i32 num_a = a;
    i32 num_b = b;
    auto [ret, flag] = num_a.overflowing_mul(num_b);
    ASSERT_FALSE(flag);
    ASSERT_EQ(ret, a * b);
  }
}

TEST(integerTest, integerOverflowingMulNoSideEffects) {
  i32 num = 10;
  i32 num1 = num;
  i32 num2 = num;
  ASSERT_EQ(num1, num);
  ASSERT_EQ(num2, num);

  auto [ret, flag] = num2.overflowing_mul(num1);
  ASSERT_FALSE(flag);
  ASSERT_EQ(ret, i32(100));
  ASSERT_EQ(num1, num);
  ASSERT_EQ(num2, num);
}

TEST(integerTest, integerSaturatingMul) {
  auto num = i8(10);
  auto num1 = i8(100);
  auto ret = num1.saturating_mul(num);
  ASSERT_EQ(ret, i8::MAX);

  i16 max_num = i16::MAX;
  ASSERT_EQ(max_num.saturating_mul(max_num), i16::MAX);
  i16 min_num = i16::MIN;
  i16 num2 = i16{-30};
  ASSERT_EQ(min_num.saturating_mul(min_num), i16::MAX);
  ASSERT_EQ(min_num.saturating_mul(num2), i16::MAX);

  constexpr int32_t min = 10;
  constexpr int32_t max = 10000;
  for (int run = 0; run < 100; run++) {
    int32_t a = random_integer(min, max);
    int32_t b = random_integer(min, max);
    i32 num_a = a;
    i32 num_b = b;
    auto ret = num_a.saturating_mul(num_b);
    ASSERT_EQ(ret, a * b);
  }
}

TEST(integerTest, integerSaturatingMulNoSideEffects) {
  i32 num = 10;
  i32 num1 = num;
  i32 num2 = num;
  ASSERT_EQ(num1, num);
  ASSERT_EQ(num2, num);

  auto ret = num2.saturating_mul(num1);
  ASSERT_EQ(ret, i32(100));
  ASSERT_EQ(num1, num);
  ASSERT_EQ(num2, num);
}

TEST(integerTest, integerWrappingMul) {
  i16 min_num = i16::MIN;
  i16 max_num = i16::MAX;
  i16 num1 = -1;
  ASSERT_EQ(min_num.wrapping_mul(num1), min_num);
  ASSERT_EQ(max_num.wrapping_mul(num1), min_num + 1);
}

TEST(integerTest, integerWrappingMulNoSideEffects) {
  i32 num = 10;
  i32 num1 = num;
  i32 num2 = num;
  ASSERT_EQ(num1, num);
  ASSERT_EQ(num2, num);

  auto ret = num2.wrapping_mul(num1);
  ASSERT_EQ(ret, i32(100));
  ASSERT_EQ(num1, num);
  ASSERT_EQ(num2, num);
}

TEST(integerTest, integerDiv) {
  constexpr int32_t min = 10;
  constexpr int32_t max = 10000;
  for (int run = 0; run < 100; run++) {
    int32_t a = random_integer(min, max);
    int32_t b = random_integer(min, max);
    i32 num_a = a;
    i32 num_b = b;
    ASSERT_EQ(i32(a / b), num_a / num_b);
    EXPECT_EQ(num_a / num_b, a / b) << a << ' ' << b << '\n';
  }
}

TEST(integerTest, integerDivNoSideEffects) {
  i32 num = 10;
  i32 num1 = num;
  i32 num2 = num;
  ASSERT_EQ(num1, num);
  ASSERT_EQ(num2, num);

  auto ret = num2 / num1;
  ASSERT_EQ(ret, i32(1));
  ASSERT_EQ(num1, num);
  ASSERT_EQ(num2, num);
}

TEST(integerTest, integerDivOverflow) {
  i8 num = -1;
  i8 num1 = i8::MIN;
  ASSERT_THROW(num1 / num, std::runtime_error);
}

TEST(integerTest, integerWrappingDiv) {
  i8 num = -1;
  i8 num1 = i8::MIN;
  ASSERT_EQ(num1.wrapping_div(num), num1);
}

TEST(integerTest, integerWrappingDivNoSideEffects) {
  i32 num = 10;
  i32 num1 = num;
  i32 num2 = num;
  ASSERT_EQ(num1, num);
  ASSERT_EQ(num2, num);

  auto ret = num2.wrapping_div(num1);
  ASSERT_EQ(ret, i32(1));
  ASSERT_EQ(num1, num);
  ASSERT_EQ(num2, num);
}

TEST(integerTest, integerCheckedDiv) {
  constexpr int32_t min = 10;
  constexpr int32_t max = 10000;
  for (int run = 0; run < 100; run++) {
    int32_t a = random_integer(min, max);
    int32_t b = random_integer(min, max);
    i32 num_a = a;
    i32 num_b = b;
    EXPECT_EQ(num_a.checked_div(num_b).value(), a / b);
  }

  i8 num = -1;
  i8 num1 = i8::MIN;
  ASSERT_EQ(num1.checked_div(num), std::nullopt);
}

TEST(integerTest, integerCheckedDivNoSideEffects) {
  i32 num = 10;
  i32 num1 = num;
  i32 num2 = num;
  ASSERT_EQ(num1, num);
  ASSERT_EQ(num2, num);

  auto ret = num2.checked_div(num1);
  ASSERT_EQ(ret.value(), i32(1));
  ASSERT_EQ(num1, num);
  ASSERT_EQ(num2, num);
}

TEST(integerTest, integerOverflowingDiv) {
  constexpr int32_t min = 10;
  constexpr int32_t max = 10000;
  for (int run = 0; run < 100; run++) {
    int32_t a = random_integer(min, max);
    int32_t b = random_integer(min, max);
    i32 num_a = a;
    i32 num_b = b;
    auto [ret, flag] = num_a.overflowing_div(num_b);
    ASSERT_FALSE(flag);
    EXPECT_EQ(ret, a / b);
  }

  i8 num = -1;
  i8 num1 = i8::MIN;
  auto [ret, flag] = num1.overflowing_div(num);
  ASSERT_TRUE(flag);
  ASSERT_EQ(ret, -1 * std::numeric_limits<int8_t>::min());
}

TEST(integerTest, integerOverflowingDivNoSideEffects) {
  i32 num = 10;
  i32 num1 = num;
  i32 num2 = num;
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
    i32 num_a = a;
    i32 num_b = b;
    auto ret = num_a.saturating_div(num_b);
    EXPECT_EQ(ret, a / b);
  }

  i8 num = -1;
  i8 num1 = i8::MIN;
  auto ret = num1.saturating_div(num);
  ASSERT_EQ(ret, i8::MIN);
}

TEST(integerTest, integerSaturatingDivNoSideEffects) {
  i32 num = 10;
  i32 num1 = num;
  i32 num2 = num;
  ASSERT_EQ(num1, num);
  ASSERT_EQ(num2, num);

  auto ret = num2.saturating_div(num1);
  ASSERT_EQ(num1, num);
  ASSERT_EQ(num2, num);
}

TEST(integerTest, integerAbs) {
  for (int32_t i = 0; i <= 1000; i += 10) {
    i32 num = i;
    ASSERT_EQ(num.abs(), num);
  }

  for (int32_t i = -1000; i <= 0; i += 10) {
    i32 num = i;
    ASSERT_EQ(num.abs(), i32(-i));
  }

  i32 max_num = i32::MAX;
  ASSERT_EQ(max_num.abs(), max_num);
  i32 min_num = i32::MIN;
  ASSERT_THROW(min_num.abs(), std::runtime_error);
}

TEST(integerTest, integerAbsNoSideEffects) {
  i32 num = 10;
  i32 tmp_num = num;
  ASSERT_EQ(num, tmp_num);

  auto _ = num.abs();
  ASSERT_EQ(num, tmp_num);
}

TEST(integerTest, integerOverflowingAbs) {
  for (int32_t i = 0; i <= 1000; i += 10) {
    i32 num = i;
    auto [ret, flag] = num.overflowing_abs();
    ASSERT_FALSE(flag);
    ASSERT_EQ(ret, num);
  }

  for (int32_t i = -1000; i <= 0; i += 10) {
    i32 num = i;
    auto [ret, flag] = num.overflowing_abs();
    ASSERT_FALSE(flag);
    ASSERT_EQ(ret, i32(-i));
  }

  i32 max_num = i32::MAX;
  auto [ret, flag] = max_num.overflowing_abs();
  ASSERT_FALSE(flag);
  ASSERT_EQ(ret, max_num);
  i32 min_num = i32::MIN;
  std::tie(ret, flag) = min_num.overflowing_abs();
  ASSERT_TRUE(flag);
  ASSERT_EQ(ret, min_num);
}

TEST(integerTest, integerOverflowingAbsNoSideEffects) {
  i32 num = 10;
  i32 tmp_num = num;
  ASSERT_EQ(num, tmp_num);

  auto [_, flag] = num.overflowing_abs();
  ASSERT_FALSE(flag);
  ASSERT_EQ(num, tmp_num);
}

TEST(integerTest, integerSaturatingAbs) {
  for (int32_t i = 0; i <= 1000; i += 10) {
    i32 num = i;
    ASSERT_EQ(num.saturating_abs(), num);
  }

  for (int32_t i = -1000; i <= 0; i += 10) {
    i32 num = i;
    ASSERT_EQ(num.saturating_abs(), i32(-i));
  }

  i32 max_num = i32::MAX;
  ASSERT_EQ(max_num.saturating_abs(), max_num);
  i32 min_num = i32::MIN;
  ASSERT_EQ(min_num.saturating_abs(), i32::MAX);
}

TEST(integerTest, integerSaturatingAbsNoSideEffects) {
  i32 num = 10;
  i32 tmp_num = num;
  ASSERT_EQ(num, tmp_num);

  auto _ = num.saturating_abs();
  ASSERT_EQ(num, tmp_num);
}

TEST(integerTest, integerCheckedAbs) {
  for (int32_t i = 0; i <= 1000; i += 10) {
    i32 num = i;
    ASSERT_EQ(num.checked_abs(), num);
  }

  for (int32_t i = -1000; i <= 0; i += 10) {
    i32 num = i;
    ASSERT_EQ(num.checked_abs(), i32(-i));
  }

  i32 max_num = i32::MAX;
  ASSERT_EQ(max_num.checked_abs(), max_num);
  i32 min_num = i32::MIN;
  ASSERT_EQ(min_num.checked_abs(), std::nullopt);
}

TEST(integerTest, integerCheckedAbsNoSideEffects) {
  i32 num = 10;
  i32 tmp_num = num;
  ASSERT_EQ(num, tmp_num);

  auto _ = num.checked_abs();
  ASSERT_EQ(num, tmp_num);
}

TEST(integerTest, integerWrappingAbs) {
  for (int32_t i = 0; i <= 1000; i += 10) {
    i32 num = i;
    ASSERT_EQ(num.wrapping_abs(), num);
  }

  for (int32_t i = -1000; i <= 0; i += 10) {
    i32 num = i;
    ASSERT_EQ(num.wrapping_abs(), i32(-i));
  }

  i32 max_num = i32::MAX;
  ASSERT_EQ(max_num.wrapping_abs(), max_num);
  i32 min_num = i32::MIN;
  ASSERT_EQ(min_num.wrapping_abs(), i32::MIN);
}

TEST(integerTest, integerWrappingAbsNoSideEffects) {
  i32 num = 10;
  i32 tmp_num = num;
  ASSERT_EQ(num, tmp_num);

  auto _ = num.wrapping_abs();
  ASSERT_EQ(num, tmp_num);
}

TEST(integerTest, integerNeg) {
  for (int32_t i = 0; i <= 1000; i += 10) {
    i32 num = i;
    ASSERT_EQ(-num, i32(-i));
  }

  for (int32_t i = -1000; i <= 0; i += 10) {
    i32 num = i;
    ASSERT_EQ(num.abs(), -num);
  }

  i32 max_num = i32::MAX;
  ASSERT_EQ(-max_num, -i32::MAX);
  i32 min_num = i32::MIN;
  ASSERT_THROW(-min_num, std::runtime_error);
}

TEST(integerTest, integerNegNoSideEffects) {
  i32 num = 10;
  i32 tmp_num = num;
  ASSERT_EQ(num, tmp_num);

  auto _ = -num;
  ASSERT_EQ(num, tmp_num);
}

TEST(integerTest, integerCheckedNeg) {
  for (int32_t i = 0; i <= 1000; i += 10) {
    i32 num = i;
    ASSERT_EQ(num.checked_neg(), i32(-i));
  }

  for (int32_t i = -1000; i <= 0; i += 10) {
    i32 num = i;
    ASSERT_EQ(num.checked_neg(), num.checked_abs());
  }

  i32 max_num = i32::MAX;
  ASSERT_EQ(max_num.checked_neg(), i32(-i32::MAX));
  i32 min_num = i32::MIN;
  ASSERT_EQ(min_num.checked_neg(), std::nullopt);
}

TEST(integerTest, integerCheckedNegNoSideEffects) {
  i32 num = 10;
  i32 tmp_num = num;
  ASSERT_EQ(num, tmp_num);

  auto _ = num.checked_neg();
  ASSERT_EQ(num, tmp_num);
}

TEST(integerTest, integerOverflowingNeg) {
  for (int32_t i = 0; i <= 1000; i += 10) {
    i32 num = i;
    auto [ret, flag] = num.overflowing_neg();
    ASSERT_FALSE(flag);
    ASSERT_EQ(ret, i32(-i));
  }

  for (int32_t i = -1000; i <= 0; i += 10) {
    i32 num = i;
    auto [ret, flag] = num.overflowing_neg();
    ASSERT_FALSE(flag);
    ASSERT_EQ(ret, num.checked_abs());
  }

  i32 max_num = i32::MAX;
  auto [ret, flag] = max_num.overflowing_neg();
  ASSERT_FALSE(flag);
  ASSERT_EQ(ret, i32(-i32::MAX));
  i32 min_num = i32::MIN;
  std::tie(ret, flag) = min_num.overflowing_neg();
  ASSERT_TRUE(flag);
  ASSERT_EQ(ret, min_num);
}

TEST(integerTest, integerOverflowingNegNoSideEffects) {
  i32 num = 10;
  i32 tmp_num = num;
  ASSERT_EQ(num, tmp_num);

  auto _ = num.overflowing_neg();
  ASSERT_EQ(num, tmp_num);
}

TEST(integerTest, integerWrappingNeg) {
  for (int32_t i = 0; i <= 1000; i += 10) {
    i32 num = i;
    ASSERT_EQ(num.wrapping_neg(), i32(-i));
  }

  for (int32_t i = -1000; i <= 0; i += 10) {
    i32 num = i;
    ASSERT_EQ(num.wrapping_neg(), num.checked_abs());
  }

  i32 max_num = i32::MAX;
  ASSERT_EQ(max_num.wrapping_neg(), i32(-i32::MAX));
  i32 min_num = i32::MIN;
  ASSERT_EQ(min_num.wrapping_neg(), min_num);
}

TEST(integerTest, integerWrappingNegNoSideEffects) {
  i32 num = 10;
  i32 tmp_num = num;
  ASSERT_EQ(num, tmp_num);

  auto _ = num.wrapping_neg();
  ASSERT_EQ(num, tmp_num);
}

TEST(integerTest, integerSaturatingNeg) {
  for (int32_t i = 0; i <= 1000; i += 10) {
    i32 num = i;
    ASSERT_EQ(num.saturating_neg(), i32(-i));
  }

  for (int32_t i = -1000; i <= 0; i += 10) {
    i32 num = i;
    ASSERT_EQ(num.saturating_neg(), num.checked_abs());
  }

  i32 max_num = i32::MAX;
  ASSERT_EQ(max_num.saturating_neg(), i32(-i32::MAX));
  i32 min_num = i32::MIN;
  ASSERT_EQ(min_num.saturating_neg(), max_num);
}

TEST(integerTest, integerSaturatingNegNoSideEffects) {
  i32 num = 10;
  i32 tmp_num = num;
  ASSERT_EQ(num, tmp_num);

  auto _ = num.saturating_neg();
  ASSERT_EQ(num, tmp_num);
}

TEST(integerTest, Hash) {
  i64 a = 4567983;
  i64 b = a;
  std::hash<i64> hasher;

  EXPECT_EQ(hasher(a), hasher(a));
  EXPECT_EQ(hasher(b), hasher(b));

  EXPECT_EQ(hasher(a), hasher(b));
}

TEST(integerTest, UnorderedSet) {
  std::unordered_set<i8> s8;
  size_t cnt = 0;
  for (i8 i = 0; i < i8::MAX; i = i.saturating_add(17)) {
    s8.insert(i);
    ++cnt;
    ASSERT_EQ(s8.size(), cnt);
    ASSERT_EQ(s8.count(i), 1);
  }
}

TEST(integerTest, ModuloArithmetic) {
  int i_a = 1234;
  i64 a = i_a;
  i64 b = a * 2;

  EXPECT_EQ(i_a % b, a);
  EXPECT_EQ(a % b, a);
  EXPECT_EQ(b % a, 0);
  EXPECT_EQ(b % i_a, 0);
  ++b;
  EXPECT_EQ(b % a, 1);
}

TEST(integerTest, BitOperations) {
  int i_a = 1;
  int i_b = 1 << 2;
  i32 a = i_a;
  i32 b = i_b;
  EXPECT_EQ(a & b, 0);
  EXPECT_EQ(i_a & b, 0);
  EXPECT_EQ(a & b, 0);
  EXPECT_EQ(a & i_b, 0);

  EXPECT_GT(a | b, a);
  EXPECT_GT(i_a | b, a);
  EXPECT_GT(a | b, b);
  EXPECT_GT(a | i_b, b);

  EXPECT_GT(a ^ b, a);
  EXPECT_GT(i_a ^ b, a);
  EXPECT_GT(a ^ b, b);
  EXPECT_GT(a ^ i_b, b);
  EXPECT_EQ(a ^ b, a + b);

  i32 c = 0;
  EXPECT_EQ(~c, -1);
}