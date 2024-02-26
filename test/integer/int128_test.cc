#include "gtest/gtest.h"

#include <numeric>

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

TEST(Int128Test, Multiplication) {
  for (int i = 0; i < 64; ++i) {
    for (int j = 0; j < 127 - i; ++j) {
      int128 a = int128(1) << i;
      int128 b = int128(1) << j;
      int128 c = int128(1) << (i + j);

      EXPECT_EQ(c, a * b);
      EXPECT_EQ(-c, -a * b);
      EXPECT_EQ(-c, a * -b);
      EXPECT_EQ(c, -a * -b);

      EXPECT_EQ(c, int128(a) *= b);
      EXPECT_EQ(-c, int128(-a) *= b);
      EXPECT_EQ(-c, int128(a) *= -b);
      EXPECT_EQ(c, int128(-a) *= -b);
    }
  }

  // pairs will not overflow signed 64-bit multiplication
  std::pair<int64_t, int64_t> small_values[] = {
      {0x5e61, 0xf29f79ca14b4},   // +, +
      {0x3e033b, -0x612c0ee549},  // +, -
      {-0x052c37e8, 0x7c728f0f},  // -, +
      {-0x7c728f0f, -0xfb17e8},   // -, -
  };

  for (const auto &[first, second] : small_values) {
    EXPECT_EQ(int128(first * second), int128(first) * int128(second));
    EXPECT_EQ(int128(first * second), int128(first) *= int128(second));

    EXPECT_EQ(make_int128(first * second, 0), make_int128(first, 0) * int128(second));
    EXPECT_EQ(make_int128(first * second, 0), make_int128(first, 0) *= int128(second));
  }

  std::pair<int64_t, int64_t> small_values2[] = {
      {0x1bb0a110, 0x31487671},
      {0x4792784e, 0x28add7d7},
      {0x7b66553a, 0x11dff8ef},
  };
  for (const auto &[first, second] : small_values2) {
    int128 a = int128(first << 32);
    int128 b = int128(second << 32);
    int128 c = make_int128(first * second, 0);

    EXPECT_EQ(c, a * b);
    EXPECT_EQ(-c, -a * b);
    EXPECT_EQ(-c, a * -b);
    EXPECT_EQ(c, -a * -b);

    EXPECT_EQ(c, int128(a) *= b);
    EXPECT_EQ(-c, int128(-a) *= b);
    EXPECT_EQ(-c, int128(a) *= -b);
    EXPECT_EQ(c, int128(-a) *= -b);
  }

  int128 large_values[] = {
      {make_int128(0xd66f061af02d0408, 0x727d2846cb475b53)},
      {make_int128(0x27b8d5ed6104452d, 0x03f8a33b0ee1df4f)},
      {-make_int128(0x621b6626b9e8d042, 0x27311ac99df00938)},
      {-make_int128(0x34e0656f1e95fb60, 0x4281cfd731257a47)},
  };
  for (auto val : large_values) {
    EXPECT_EQ(0, 0 * val);
    EXPECT_EQ(0, val * 0);
    EXPECT_EQ(0, int128(0) *= val);
    EXPECT_EQ(0, val *= 0);

    EXPECT_EQ(val, 1 * val);
    EXPECT_EQ(val, val * 1);
    EXPECT_EQ(val, int128(1) *= val);
    EXPECT_EQ(val, val *= 1);

    EXPECT_EQ(-val, -1 * val);
    EXPECT_EQ(-val, val * -1);
    EXPECT_EQ(-val, int128(-1) * val);
    EXPECT_EQ(-val, val *= -1);
  }

  // Manually calculated random large value cases
  EXPECT_EQ(make_int128(0xcd0efd3442219bb, 0xde47c05bcd9df6e1),
            make_int128(0x7c6448, 0x3bc4285c47a9d253) * 0x1a6037537b);
  EXPECT_EQ(make_int128(0x19c8b7620b507dc4, 0xfec042b71a5f29a4),
            -0x3e39341147 * -make_int128(0x6a14b2, 0x5ed34cca42327b3c));

  EXPECT_EQ(make_int128(0xcd0efd3442219bb, 0xde47c05bcd9df6e1),
            make_int128(0x7c6448, 0x3bc4285c47a9d253) *= int128(0x1a6037537b));
  EXPECT_EQ(make_int128(0x19c8b7620b507dc4, 0xfec042b71a5f29a4),
            int128(-0x3e39341147) *= -make_int128(0x6a14b2, 0x5ed34cca42327b3c));
}

TEST(Int128Test, DivisionAndModulo) {
  std::pair<int64_t, int64_t> small_pairs[] = {
      {0x15f2a64138, 0x67da05},    {0x5e56d194af43045f, 0xcf1543fb99},
      {0x15e61ed052036a, -0xc8e6}, {0x88125a341e85, -0xd23fb77683},
      {-0xc06e20, 0x5a},           {-0x4f100219aea3e85d, 0xdcc56cb4efe993},
      {-0x168d629105, -0xa7},      {-0x7b44e92f03ab2375, -0x6516},
  };
  for (const auto &[first, second] : small_pairs) {
    int128 dividend = first;
    int128 divisor = second;
    int64_t quotient = first / second;
    int64_t remainder = first % second;

    EXPECT_EQ(quotient, dividend / divisor);
    EXPECT_EQ(quotient, int128(dividend) /= divisor);
    EXPECT_EQ(remainder, dividend % divisor);
    EXPECT_EQ(remainder, int128(dividend) %= divisor);
  }

  // randomly generated large values for 0, 1 and -1
  int128 values[] = {
      make_int128(0x63d26ee688a962b2, 0x9e1411abda5c1d70),
      make_int128(0x152f385159d6f986, 0xbf8d48ef63da395d),
      -make_int128(0x3098d7567030038c, 0x14e7a8a098dc2164),
      -make_int128(0x49a037aca35c809f, 0xa6a87525480ef330),
  };
  for (auto val : values) {
    SCOPED_TRACE(::testing::Message() << "val: " << val);

    EXPECT_EQ(0, 0 / val);
    EXPECT_EQ(0, int128(0) /= val);
    EXPECT_EQ(0, 0 % val);
    EXPECT_EQ(0, int128(0) %= val);

    EXPECT_EQ(val, val / 1);
    EXPECT_EQ(val, int128(val) /= 1);
    EXPECT_EQ(0, val % 1);
    EXPECT_EQ(0, int128(val) %= 1);

    EXPECT_EQ(-val, val / -1);
    EXPECT_EQ(-val, int128(val) /= -1);
    EXPECT_EQ(0, val % -1);
    EXPECT_EQ(0, int128(val) %= -1);
  }

  // Min and max
  EXPECT_EQ(0, int128_max() / int128_min());
  EXPECT_EQ(int128_max(), int128_max() % int128_min());
  EXPECT_EQ(-1, int128_min() / int128_max());
  EXPECT_EQ(-1, int128_min() % int128_max());

  // Power of two division and modulo of random large values
  int128 positive_values[] = {
      make_int128(0x21e1a1cc69574620, 0xe7ac447fab2fc869),
      make_int128(0x32c2ff3ab89e66e8, 0x03379a613fd1ce74),
      make_int128(0x6f32ca786184dcaf, 0x046f9c9ecb3a9ce1),
      make_int128(0x1aeb469dd990e0ee, 0xda2740f243cd37eb),
  };
  for (auto val : positive_values) {
    for (int i = 0; i < 127; ++i) {
      int128 power_of_two = int128(1) << i;
      EXPECT_EQ(val >> i, val / power_of_two);
      EXPECT_EQ(val >> i, int128(val) /= power_of_two);
      EXPECT_EQ(val & (power_of_two - 1), val % power_of_two);
      EXPECT_EQ(val & (power_of_two - 1), int128(val) %= power_of_two);
    }
  }

  // Manually calculated random large value cases
  struct DivisionModCase {
    int128 dividend;
    int128 divisor;
    int128 quotient;
    int128 remainder;
  };

  DivisionModCase cases[] = {
      {make_int128(0x6ada48d489007966, 0x3c9c5c98150d5d69), make_int128(0x8bc308fb, 0x8cb9cc9a3b803344), 0xc3b87e08,
       make_int128(0x1b7db5e1, 0xd9eca34b7af04b49)},
      {make_int128(0xd6946511b5b, 0x4886c5c96546bf5f), -make_int128(0x263b, 0xfd516279efcfe2dc), -0x59cbabf0,
       make_int128(0x622, 0xf462909155651d1f)},
      {-make_int128(0x33db734f9e8d1399, 0x8447ac92482bca4d), 0x37495078240, -make_int128(0xf01f1, 0xbc0368bf9a77eae8),
       -0x21a508f404d},
      {-make_int128(0x13f837b409a07e7d, 0x7fc8e248a7d73560), -0x1b9f, make_int128(0xb9157556d724, 0xb14f635714d7563e),
       -0x1ade},
  };
  for (const auto &[dividend, divisor, quotient, remainder] : cases) {
    EXPECT_EQ(quotient, dividend / divisor);
    EXPECT_EQ(quotient, int128(dividend) /= divisor);
    EXPECT_EQ(remainder, dividend % divisor);
    EXPECT_EQ(remainder, int128(dividend) %= divisor);
  }
}

TEST(Int128Test, BitwiseLogic) {
  EXPECT_EQ(int128(-1), ~int128(0));

  int128 values[] = {
      0,
      -1,
      0xde400bee05c3ff6b,
      make_int128(0x7f32178dd81d634a, 0),
      make_int128(0xaf539057055613a, 0x7d104d7d946c2e4d),
  };
  for (auto val : values) {
    SCOPED_TRACE(::testing::Message() << "val: " << val);

    EXPECT_EQ(val, ~~val);
    EXPECT_EQ(val, val & val);
    EXPECT_EQ(val, val | val);
    EXPECT_EQ(0, val ^ val);

    EXPECT_EQ(val, int128(val) &= val);
    EXPECT_EQ(val, int128(val) |= val);
    EXPECT_EQ(0, int128(val) ^= val);

    EXPECT_EQ(val, val | 0);
    EXPECT_EQ(0, val & 0);
    EXPECT_EQ(val, val ^ 0);

    EXPECT_EQ(int128(-1), val | int128(-1));
    EXPECT_EQ(val, val & int128(-1));
    EXPECT_EQ(~val, val ^ int128(-1));
  }

  std::pair<int64_t, int64_t> pairs64[] = {
      {0x7f86797f5e991af4, 0x1ee30494fb007c97}, {0x0b278282bacf01af, 0x58780e0a57a49e86},
      {0x059f266ccb93a666, 0x3d5b731bae9286f5}, {0x63c0c4820f12108c, 0x58166713c12e1c3a},
      {0x381488bb2ed2a66e, 0x2220a3eb76a3698c}, {0x2a0a0dfb81e06f21, 0x4b60585927f5523c},
      {0x555b1c3a03698537, 0x25478cd19d8e53cb}, {0x4750f6f27d779225, 0x16397553c6ff05fc},
  };
  for (const auto &[first, second] : pairs64) {
    SCOPED_TRACE(::testing::Message() << "first: " << first << ", second: " << second);

    EXPECT_EQ(make_int128(~first, ~second), ~make_int128(first, second));
    EXPECT_EQ(int128(first & second), int128(first) & int128(second));
    EXPECT_EQ(int128(first | second), int128(first) | int128(second));
    EXPECT_EQ(int128(first ^ second), int128(first) ^ int128(second));

    EXPECT_EQ(int128(first & second), int128(first) &= int128(second));
    EXPECT_EQ(int128(first | second), int128(first) |= int128(second));
    EXPECT_EQ(int128(first ^ second), int128(first) ^= int128(second));

    EXPECT_EQ(make_int128(first & second, 0), make_int128(first, 0) & make_int128(second, 0));
    EXPECT_EQ(make_int128(first | second, 0), make_int128(first, 0) | make_int128(second, 0));
    EXPECT_EQ(make_int128(first ^ second, 0), make_int128(first, 0) ^ make_int128(second, 0));

    EXPECT_EQ(make_int128(first & second, 0), make_int128(first, 0) &= make_int128(second, 0));
    EXPECT_EQ(make_int128(first | second, 0), make_int128(first, 0) |= make_int128(second, 0));
    EXPECT_EQ(make_int128(first ^ second, 0), make_int128(first, 0) ^= make_int128(second, 0));
  }
}

TEST(Int128Test, BitwiseShiftTest) {
  for (int i = 0; i < 64; ++i) {
    for (int j = 0; j <= i; ++j) {
      SCOPED_TRACE(::testing::Message() << "i: " << i << ", j: " << j);
      EXPECT_EQ(uint64_t(1) << i, int128(uint64_t(1) << j) << (i - j));
      EXPECT_EQ(uint64_t(1) << i, int128(uint64_t(1) << j) <<= (i - j));
    }
  }
}

TEST(Int128Test, NumericLimits) {
  static_assert(std::numeric_limits<int128>::is_specialized, "std::numeric_limits<int128> must be specialized");
  static_assert(std::numeric_limits<int128>::is_signed, "std::numeric_limits<int128> must be signed");
  static_assert(std::numeric_limits<int128>::is_integer, "std::numeric_limits<int128> must be integer");

  EXPECT_EQ(static_cast<int>(127 * std::log10(2)), std::numeric_limits<int128>::digits10);
  EXPECT_EQ(int128_min(), std::numeric_limits<int128>::min());
  EXPECT_EQ(int128_min(), std::numeric_limits<int128>::lowest());
  EXPECT_EQ(int128_max(), std::numeric_limits<int128>::max());
}
