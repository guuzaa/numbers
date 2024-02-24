#include "gtest/gtest.h"

#include <numeric>
#include <vector>

#include "int128.hh"

using namespace numbers;

TEST(Int128Test, TrivalTraits) {
  static_assert(std::is_trivially_default_constructible<int128>::value,
                "int128 should be trivially default constructible");
  static_assert(std::is_trivially_copy_constructible<int128>::value, "int128 should be trivially copy constructible");
  static_assert(std::is_trivially_copy_assignable<int128>::value, "int128 should be trivially copy assignable");
  static_assert(std::is_trivially_destructible<int128>::value, "int128 should be trivially destructible");
}

typedef ::testing::Types<bool, char, signed char, unsigned char, char16_t, char32_t, short, unsigned short, int,
                         unsigned int, long, unsigned long, long long, unsigned long long>
    IntegerTypes;

template <typename T>
class Int128TraitsTest : public ::testing::Test {};

TYPED_TEST_SUITE(Int128TraitsTest, IntegerTypes);

TYPED_TEST(Int128TraitsTest, ConstructAssignTest) {
  static_assert(std::is_constructible<int128, TypeParam>::value,
                "numbers::int128 must be constructible from TypeParam");
  static_assert(std::is_assignable<int128 &, TypeParam>::value, "numbers::int128 must be assignable from TypeParam");
  static_assert(!std::is_assignable<TypeParam &, int128>::value,
                "TypeParam must not be assignable from numbers::int128");
}

TEST(Int128Test, BoolConversion) {
  EXPECT_FALSE(int128(0));
  for (int i = 0; i < 64; ++i) {
    EXPECT_TRUE(make_int128(0, uint64_t(1) << i));
  }

  for (int i = 0; i < 63; ++i) {
    EXPECT_TRUE(make_int128(int64_t(1) << i, 0));
  }

  EXPECT_TRUE(int128::MIN);
  EXPECT_TRUE(int128::MAX);
  EXPECT_EQ(int128(1), int128(true));
  EXPECT_EQ(int128(0), int128(false));
}

template <typename T>
class Int128ConversionTest : public ::testing::Test {};

TYPED_TEST_SUITE(Int128ConversionTest, IntegerTypes);

TYPED_TEST(Int128ConversionTest, Int128Conversion) {
  EXPECT_EQ(TypeParam{0}, static_cast<TypeParam>(int128(0)));
  EXPECT_EQ(std::numeric_limits<TypeParam>::min(),
            static_cast<TypeParam>(int128(std::numeric_limits<TypeParam>::min())));
  EXPECT_EQ(std::numeric_limits<TypeParam>::max(),
            static_cast<TypeParam>(int128(std::numeric_limits<TypeParam>::max())));
}

TEST(Int128Test, MakeInt128) {
  EXPECT_EQ(int128(-1), make_int128(-1, -1));
  EXPECT_EQ(int128(-31), make_int128(-1, -31));
  EXPECT_EQ(int128(std::numeric_limits<int64_t>::min()), make_int128(-1, std::numeric_limits<int64_t>::min()));
  EXPECT_EQ(int128(0), make_int128(0, 0));
  EXPECT_EQ(int128(1), make_int128(0, 1));
  EXPECT_EQ(int128(345), make_int128(0, 345));
  EXPECT_EQ(int128(std::numeric_limits<int64_t>::max()), make_int128(0, std::numeric_limits<int64_t>::max()));
}

TEST(Int128Test, int128HighLowTest) {
  struct HighLowPair {
    int64_t high;
    uint64_t low;
  };

  HighLowPair values[]{{0, 0}, {0, 1}, {1, 0}, {1, 1}, {1234, 5678}, {-8765, 4321}};
  for (const auto &[high, low] : values) {
    int128 val = make_int128(high, low);
    EXPECT_EQ(low, int128_low64(val));
    EXPECT_EQ(high, int128_high64(val));
  }
}

TEST(Int128Test, int128LimitsTest) {
  EXPECT_EQ(int128::MAX, make_int128(0x7fffffffffffffff, 0xffffffffffffffff));
  EXPECT_EQ(int128::MAX, ~int128::MIN);
}

TEST(Int128Test, int128Constexpr) {
  constexpr int128 zero = int128();
  constexpr int128 one = 1;
  constexpr int128 minus_two = -2;
  constexpr int128 min = int128_min();
  constexpr int128 max = int128_max();
  EXPECT_EQ(zero, int128(0));
  EXPECT_EQ(one, int128(1));
  EXPECT_EQ(minus_two, make_int128(-1, -2));
  EXPECT_GT(max, one);
  EXPECT_LT(min, minus_two);
}

TEST(Int128Test, int128Comparsion) {
  struct TestCase {
    int128 smaller;
    int128 larger;
  };

  TestCase cases[] = {
      {int128(0), int128(123)},
      {make_int128(-12, 34), make_int128(12, 34)},
      {make_int128(1, 1000), make_int128(1000, 1)},
      {make_int128(1000, 1000), make_int128(1010, 1000)},
      {make_int128(-1000, 1000), make_int128(-1, 1)},
  };

  for (const auto &[smaller, larger] : cases) {
    EXPECT_TRUE(smaller < larger);
    EXPECT_LT(smaller, larger);
    EXPECT_TRUE(smaller <= larger);
    EXPECT_LE(smaller, larger);
    EXPECT_TRUE(larger > smaller);
    EXPECT_GT(larger, smaller);
    EXPECT_TRUE(larger >= smaller);
    EXPECT_GE(larger, smaller);
    EXPECT_EQ(smaller, smaller);
    EXPECT_TRUE(smaller == smaller);
    EXPECT_EQ(larger, larger);
    EXPECT_TRUE(larger == larger);
    EXPECT_NE(smaller, larger);
    EXPECT_FALSE(smaller == larger);
  }
}

TEST(Int128Test, int128UnaryPlus) {
  int64_t values64[] = {0, 1, 1000, 1234, std::numeric_limits<int64_t>::max()};
  for (auto val : values64) {
    EXPECT_EQ(int128(val), +int128(val));
    EXPECT_EQ(int128(-val), +int128(-val));
    EXPECT_EQ(make_int128(val, 0), +make_int128(val, 0));
    EXPECT_EQ(make_int128(-val, 0), +make_int128(-val, 0));
  }
}

TEST(Int128Test, int128UnaryNegation) {
  int64_t values64[] = {0, 1, 1000, 1234, std::numeric_limits<int64_t>::max()};
  for (auto val : values64) {
    EXPECT_EQ(int128(-val), -int128(val));
    EXPECT_EQ(int128(val), -int128(-val));
    EXPECT_EQ(make_int128(-val, 0), -make_int128(val, 0));
    EXPECT_EQ(make_int128(val, 0), -make_int128(-val, 0));
  }
}

TEST(Int128Test, int128LogicNot) {
  EXPECT_TRUE(!int128(0));
  for (int i = 0; i < 64; ++i) {
    EXPECT_FALSE(!make_int128(0, uint64_t(1) << i));
  }
  for (int i = 0; i < 63; ++i) {
    EXPECT_FALSE(!make_int128(int64_t(1) << i, 0));
  }
}

TEST(Int128Test, int128AdditionSubtraction) {
  // pairs that will not cause overflow / underflow.
  std::pair<int64_t, int64_t> cases[]{
      {0, 0},                            // 0, 0
      {0, 2945781290834},                // 0, +
      {190835761924, 0},                 // +, 0
      {0, -1204895918245},               // 0, -
      {-2957928523560, 0},               // -, 0
      {8945781290834, 2945781290834},    // +, +
      {-45781290834, -1204895918245},    // - ,-
      {2957928523560, -2945781290834},   // +, -
      {-88235237438467, 1592369524573},  // -, +
  };

  for (const auto &[first, second] : cases) {
    EXPECT_EQ(int128(first + second), int128(first) + int128(second));
    EXPECT_EQ(int128(second + first), int128(second) += int128(first));

    EXPECT_EQ(int128(first - second), int128(first) - int128(second));
    EXPECT_EQ(int128(second - first), int128(second) -= int128(first));

    EXPECT_EQ(make_int128(second + first, 0), make_int128(second, 0) + make_int128(first, 0));
    EXPECT_EQ(make_int128(first + second, 0), make_int128(first, 0) += make_int128(second, 0));

    EXPECT_EQ(make_int128(second - first, 0), make_int128(second, 0) - make_int128(first, 0));
    EXPECT_EQ(make_int128(first - second, 0), make_int128(first, 0) -= make_int128(second, 0));
  }

  // check postive carry
  EXPECT_EQ(make_int128(31, 0), make_int128(20, 1) + make_int128(10, std::numeric_limits<uint64_t>::max()));
}

TEST(Int128Test, IncrementDecrement) {
  int128 val = 0;
  EXPECT_EQ(0, val++);
  EXPECT_EQ(1, val);
  EXPECT_EQ(1, val--);
  EXPECT_EQ(0, val);
  EXPECT_EQ(-1, --val);
  EXPECT_EQ(-1, val);
  EXPECT_EQ(0, ++val);
  EXPECT_EQ(0, val);
}