//
// Copyright 2017 The Abseil Authors.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//      https://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//
// modified from abseil-cpp guuzaa

#ifndef NUMBERS_INT128_HH
#define NUMBERS_INT128_HH

#include <cassert>
#include <cmath>
#include <limits>
#include <string>

#include "internal/config.h"

#if defined(_MSC_VER) && defined(_M_X64) && !defined(_M_ARM64EC)
#include <intrin.h>
#pragma intrinsic(_umul128)
#endif

namespace numbers {
class int128;
class uint128 {
 public:
  uint128() = default;
  // Constructors from arithmetic types
  constexpr uint128(int v);
  constexpr uint128(unsigned int v);
  constexpr uint128(long v);
  constexpr uint128(unsigned long v);
  constexpr uint128(long long v);
  constexpr uint128(unsigned long long v);
  constexpr uint128(int128 v);

#ifdef NUMBERS_HAVE_INTRINSTIC_INT128
  constexpr uint128(__int128 v);
  constexpr uint128(unsigned __int128 v);
#endif

  explicit uint128(float v);
  explicit uint128(double v);
  explicit uint128(long double v);

  // Assignment operators from arithmetic types
  uint128 &operator=(int v);
  uint128 &operator=(unsigned int v);
  uint128 &operator=(long v);
  uint128 &operator=(unsigned long v);
  uint128 &operator=(long long v);
  uint128 &operator=(unsigned long long v);
  uint128 &operator=(int128 v);

  // Conversion operators to other arithmetic types
  constexpr explicit operator bool() const;
  constexpr explicit operator char() const;
  constexpr explicit operator signed char() const;
  constexpr explicit operator unsigned char() const;
  constexpr explicit operator char16_t() const;
  constexpr explicit operator char32_t() const;
  constexpr explicit operator short() const;
  constexpr explicit operator unsigned short() const;
  constexpr explicit operator int() const;
  constexpr explicit operator unsigned int() const;
  constexpr explicit operator long() const;
  constexpr explicit operator unsigned long() const;
  constexpr explicit operator long long() const;
  constexpr explicit operator unsigned long long() const;

#ifdef NUMBERS_HAVE_INTRINSTIC_INT128
  constexpr explicit operator __int128() const;
  constexpr explicit operator unsigned __int128() const;
#endif

  explicit operator float() const;
  explicit operator double() const;
  explicit operator long double() const;

  // Trivial copy constructor, assignment operator and destructor.

  // Arithmetic operators.
  uint128 &operator+=(uint128 other);
  uint128 &operator-=(uint128 other);
  uint128 &operator*=(uint128 other);
  // Long division/modulo for uint128.
  uint128 &operator/=(uint128 other);
  uint128 &operator%=(uint128 other);
  uint128 operator++(int);
  uint128 operator--(int);
  uint128 &operator<<=(int);
  uint128 &operator>>=(int);
  uint128 &operator&=(uint128 other);
  uint128 &operator|=(uint128 other);
  uint128 &operator^=(uint128 other);
  uint128 &operator++();
  uint128 &operator--();

  // uint128_low64()
  //
  // Returns the lower 64-bit value of a `uint128` value.
  friend constexpr uint64_t uint128_low64(uint128 v);

  // uint128_high64()
  //
  // Returns the higher 64-bit value of a `uint128` value.
  friend constexpr uint64_t uint128_high64(uint128 v);

  // MakeUInt128()
  //
  // Constructs a `uint128` numeric value from two 64-bit unsigned integers.
  // Note that this factory function is the only way to construct a `uint128`
  // from integer values greater than 2^64.
  //
  // Example:
  //
  //   numbers::uint128 big = numbers::make_uint128(1, 0);
  friend constexpr uint128 make_uint128(uint64_t high, uint64_t low);

  // uint128_max()
  //
  // Returns the highest value for a 128-bit unsigned integer.
  friend constexpr uint128 uint128_max();

 private:
  constexpr uint128(uint64_t high, uint64_t low);

  std::string to_string() const;

  // little endian
  uint64_t lo_;
  uint64_t hi_;
};

std::ostream &operator<<(std::ostream &os, uint128 v);

constexpr uint128 uint128_max() {
  return uint128((std::numeric_limits<uint64_t>::max)(), (std::numeric_limits<uint64_t>::max)());
}
}  // namespace numbers

// Specialized numeric_limits for uint128.
namespace std {
template <>
struct hash<numbers::uint128> {
  size_t operator()(const numbers::uint128 &obj) const {
    return (std::hash<uint64_t>()(uint128_high64(obj)) << 1) ^ std::hash<uint64_t>()(uint128_low64(obj));
  }
};

template <>
class numeric_limits<numbers::uint128> {
 public:
  static constexpr bool is_specialized = true;
  static constexpr bool is_signed = false;
  static constexpr bool is_integer = true;
  static constexpr bool is_exact = true;
  static constexpr bool has_infinity = false;
  static constexpr bool has_quiet_NaN = false;
  static constexpr bool has_signaling_NaN = false;
  static constexpr float_denorm_style has_denorm = denorm_absent;
  static constexpr bool has_denorm_loss = false;
  static constexpr float_round_style round_style = round_toward_zero;
  static constexpr bool is_iec559 = false;
  static constexpr bool is_bounded = true;
  static constexpr bool is_modulo = true;
  static constexpr int digits = 128;
  static constexpr int digits10 = 38;
  static constexpr int max_digits10 = 0;
  static constexpr int radix = 2;
  static constexpr int min_exponent = 0;
  static constexpr int min_exponent10 = 0;
  static constexpr int max_exponent = 0;
  static constexpr int max_exponent10 = 0;
#ifdef NUMBERS_HAVE_INTRINSTIC_INT128
  static constexpr bool traps = numeric_limits<unsigned __int128>::traps;
#else
  static constexpr bool traps = numeric_limits<uint64_t>::traps;
#endif
  static constexpr bool tinyness_before = false;

  static constexpr numbers::uint128(min)() { return 0; }
  static constexpr numbers::uint128 lowest() { return 0; }
  static constexpr numbers::uint128(max)() { return numbers::uint128_max(); }
  static constexpr numbers::uint128 epsilon() { return 0; }
  static constexpr numbers::uint128 round_error() { return 0; }
  static constexpr numbers::uint128 infinity() { return 0; }
  static constexpr numbers::uint128 quiet_NaN() { return 0; }
  static constexpr numbers::uint128 signaling_NaN() { return 0; }
  static constexpr numbers::uint128 denorm_min() { return 0; }
};
}  // namespace std

namespace numbers {

class int128 {
 public:
  int128() = default;

  // Constructors from arithmetic types
  constexpr int128(int v);
  constexpr int128(unsigned int v);
  constexpr int128(long v);
  constexpr int128(unsigned long v);
  constexpr int128(long long v);
  constexpr int128(unsigned long long v);
  constexpr explicit int128(uint128 v);
#ifdef NUMBERS_HAVE_INTRINSTIC_INT128
  constexpr int128(__int128 v);
  constexpr explicit int128(unsigned __int128 v);
#endif

  explicit int128(float v);
  explicit int128(double v);
  explicit int128(long double v);

  // Assignment operators from arithmetic types
  int128 &operator=(int v);
  int128 &operator=(unsigned int v);
  int128 &operator=(long v);
  int128 &operator=(unsigned long v);
  int128 &operator=(long long v);
  int128 &operator=(unsigned long long v);
#ifdef NUMBERS_HAVE_INTRINSTIC_INT128
  int128 &operator=(__int128 v);
#endif

  // Conversion operators to other arithmetic types
  constexpr explicit operator bool() const;
  constexpr explicit operator char() const;
  constexpr explicit operator signed char() const;
  constexpr explicit operator unsigned char() const;
  constexpr explicit operator char16_t() const;
  constexpr explicit operator char32_t() const;
  constexpr explicit operator short() const;

  constexpr explicit operator unsigned short() const;
  constexpr explicit operator int() const;
  constexpr explicit operator unsigned int() const;
  constexpr explicit operator long() const;

  constexpr explicit operator unsigned long() const;

  constexpr explicit operator long long() const;

  constexpr explicit operator unsigned long long() const;
#ifdef NUMBERS_HAVE_INTRINSTIC_INT128
  constexpr explicit operator __int128() const;
  constexpr explicit operator unsigned __int128() const;
#endif

  explicit operator float() const;
  explicit operator double() const;
  explicit operator long double() const;

  // Trivial copy constructor, assignment operator and destructor.

  // Arithmetic operators
  int128 &operator+=(int128 other);
  int128 &operator-=(int128 other);
  int128 &operator*=(int128 other);
  int128 &operator/=(int128 other);
  int128 &operator%=(int128 other);
  int128 operator++(int);  // postfix increment: i++
  int128 operator--(int);  // postfix decrement: i--
  int128 &operator++();    // prefix increment:  ++i
  int128 &operator--();    // prefix decrement:  --i
  int128 &operator&=(int128 other);
  int128 &operator|=(int128 other);
  int128 &operator^=(int128 other);
  int128 &operator<<=(int amount);
  int128 &operator>>=(int amount);

  // int128_low64()
  //
  // Returns the lower 64-bit value of a `int128` value.
  friend constexpr uint64_t int128_low64(int128 v);

  // int128_high64()
  //
  // Returns the higher 64-bit value of a `int128` value.
  friend constexpr int64_t int128_high64(int128 v);

  // make_int128()
  //
  // Constructs a `int128` numeric value from two 64-bit integers. Note that
  // signedness is conveyed in the upper `high` value.
  //
  //   (numbers::int128(1) << 64) * high + low
  //
  // Note that this factory function is the only way to construct a `int128`
  // from integer values greater than 2^64 or less than -2^64.
  //
  // Example:
  //
  //   numbers::int128 big = numbers::make_int128(1, 0);
  //   numbers::int128 big_n = numbers::make_int128(-1, 0);
  friend constexpr int128 make_int128(int64_t high, uint64_t low);

  // Int128Max()
  //
  // Returns the maximum value for a 128-bit signed integer.
  friend constexpr int128 int128_max();

  // Int128Min()
  //
  // Returns the minimum value for a 128-bit signed integer.
  friend constexpr int128 int128_min();

  static int128 MAX;
  static int128 MIN;

 private:
  constexpr int128(int64_t high, uint64_t low);

  std::string to_string() const;

#ifdef NUMBERS_HAVE_INTRINSTIC_INT128
  __int128 v_;
#else
  // little endian
  uint64_t lo_;
  int64_t hi_;
#endif
};

std::ostream &operator<<(std::ostream &os, int128 v);

constexpr int128 int128_max() {
  return int128((std::numeric_limits<int64_t>::max)(), (std::numeric_limits<uint64_t>::max)());
}

constexpr int128 int128_min() { return int128((std::numeric_limits<int64_t>::min)(), 0); }

inline int128 int128::MAX = int128_max();

inline int128 int128::MIN = int128_min();

}  // namespace numbers

namespace std {
template <>
struct is_signed<numbers::int128> : std::true_type {};

template <>
struct hash<numbers::int128> {
  size_t operator()(const numbers::int128 &obj) const {
    return std::hash<int64_t>()(int128_high64(obj) << 1) ^ std::hash<uint64_t>()(int128_low64(obj));
  }
};

template <>
class numeric_limits<numbers::int128> {
 public:
  static constexpr bool is_specialized = true;
  static constexpr bool is_signed = true;
  static constexpr bool is_integer = true;
  static constexpr bool is_exact = true;
  static constexpr bool has_infinity = false;
  static constexpr bool has_quiet_NaN = false;
  static constexpr bool has_signaling_NaN = false;
  static constexpr float_denorm_style has_denorm = denorm_absent;
  static constexpr bool has_denorm_loss = false;
  static constexpr float_round_style round_style = round_toward_zero;
  static constexpr bool is_iec559 = false;
  static constexpr bool is_bounded = true;
  static constexpr bool is_modulo = false;
  static constexpr int digits = 127;
  static constexpr int digits10 = 38;
  static constexpr int max_digits10 = 0;
  static constexpr int radix = 2;
  static constexpr int min_exponent = 0;
  static constexpr int min_exponent10 = 0;
  static constexpr int max_exponent = 0;
  static constexpr int max_exponent10 = 0;
#ifdef NUMBERS_HAVE_INTRINSTIC_INT128
  static constexpr bool traps = numeric_limits<__int128>::traps;
#else
  static constexpr bool traps = numeric_limits<uint64_t>::traps;
#endif
  static constexpr bool tinyness_before = false;

  static constexpr numbers::int128(min)() { return numbers::int128_min(); }
  static constexpr numbers::int128 lowest() { return numbers::int128_min(); }
  static constexpr numbers::int128(max)() { return numbers::int128_max(); }
  static constexpr numbers::int128 epsilon() { return 0; }
  static constexpr numbers::int128 round_error() { return 0; }
  static constexpr numbers::int128 infinity() { return 0; }
  static constexpr numbers::int128 quiet_NaN() { return 0; }
  static constexpr numbers::int128 signaling_NaN() { return 0; }
  static constexpr numbers::int128 denorm_min() { return 0; }
};
}  // namespace std

namespace numbers {
constexpr uint128 make_uint128(uint64_t high, uint64_t low) { return uint128(high, low); }

// Assignments from integer types.

inline uint128 &uint128::operator=(int v) { return *this = uint128(v); }

inline uint128 &uint128::operator=(unsigned int v) { return *this = uint128(v); }

inline uint128 &uint128::operator=(long v) { return *this = uint128(v); }

inline uint128 &uint128::operator=(unsigned long v) { return *this = uint128(v); }

inline uint128 &uint128::operator=(long long v) { return *this = uint128(v); }

inline uint128 &uint128::operator=(unsigned long long v) { return *this = uint128(v); }

inline uint128 &uint128::operator=(int128 v) { return *this = uint128(v); }

// Arithmetic operators.

constexpr uint128 operator<<(uint128 lhs, int amount);
constexpr uint128 operator>>(uint128 lhs, int amount);
constexpr uint128 operator+(uint128 lhs, uint128 rhs);
constexpr uint128 operator-(uint128 lhs, uint128 rhs);
uint128 operator*(uint128 lhs, uint128 rhs);
uint128 operator/(uint128 lhs, uint128 rhs);
uint128 operator%(uint128 lhs, uint128 rhs);

inline uint128 &uint128::operator<<=(int amount) {
  *this = *this << amount;
  return *this;
}

inline uint128 &uint128::operator>>=(int amount) {
  *this = *this >> amount;
  return *this;
}

inline uint128 &uint128::operator+=(uint128 other) {
  *this = *this + other;
  return *this;
}

inline uint128 &uint128::operator-=(uint128 other) {
  *this = *this - other;
  return *this;
}

inline uint128 &uint128::operator*=(uint128 other) {
  *this = *this * other;
  return *this;
}

inline uint128 &uint128::operator/=(uint128 other) {
  *this = *this / other;
  return *this;
}

inline uint128 &uint128::operator%=(uint128 other) {
  *this = *this % other;
  return *this;
}

constexpr uint64_t uint128_low64(uint128 v) { return v.lo_; }

constexpr uint64_t uint128_high64(uint128 v) { return v.hi_; }

// little endian

constexpr uint128::uint128(uint64_t high, uint64_t low) : lo_{low}, hi_{high} {}

constexpr uint128::uint128(int v)
    : lo_{static_cast<uint64_t>(v)}, hi_{v < 0 ? (std::numeric_limits<uint64_t>::max)() : 0} {}

constexpr uint128::uint128(long v)
    : lo_{static_cast<uint64_t>(v)}, hi_{v < 0 ? (std::numeric_limits<uint64_t>::max)() : 0} {}

constexpr uint128::uint128(long long v)
    : lo_{static_cast<uint64_t>(v)}, hi_{v < 0 ? (std::numeric_limits<uint64_t>::max)() : 0} {}

constexpr uint128::uint128(unsigned int v) : lo_{v}, hi_{0} {}

constexpr uint128::uint128(unsigned long v) : lo_{v}, hi_{0} {}

constexpr uint128::uint128(unsigned long long v) : lo_{v}, hi_{0} {}

constexpr uint128::uint128(int128 v) : lo_{int128_low64(v)}, hi_{static_cast<uint64_t>(int128_high64(v))} {}

#ifdef NUMBERS_HAVE_INTRINSTIC_INT128
constexpr uint128::uint128(__int128 v)
    : lo_{static_cast<uint64_t>(v & ~uint64_t{0})},
      hi_{static_cast<uint64_t>(static_cast<unsigned __int128>(v) >> 64)} {}

constexpr uint128::uint128(unsigned __int128 v)
    : lo_{static_cast<uint64_t>(v & ~uint64_t{0})}, hi_{static_cast<uint64_t>(v >> 64)} {}
#endif

// Conversion operators to integer types.

constexpr uint128::operator bool() const { return lo_ || hi_; }

constexpr uint128::operator char() const { return static_cast<char>(lo_); }

constexpr uint128::operator signed char() const { return static_cast<signed char>(lo_); }

constexpr uint128::operator unsigned char() const { return static_cast<unsigned char>(lo_); }

constexpr uint128::operator char16_t() const { return static_cast<char16_t>(lo_); }

constexpr uint128::operator char32_t() const { return static_cast<char32_t>(lo_); }

constexpr uint128::operator short() const { return static_cast<short>(lo_); }

constexpr uint128::operator unsigned short() const { return static_cast<unsigned short>(lo_); }

constexpr uint128::operator int() const { return static_cast<int>(lo_); }

constexpr uint128::operator unsigned int() const { return static_cast<unsigned int>(lo_); }

constexpr uint128::operator long() const { return static_cast<long>(lo_); }

constexpr uint128::operator unsigned long() const { return static_cast<unsigned long>(lo_); }

constexpr uint128::operator long long() const { return static_cast<long long>(lo_); }

constexpr uint128::operator unsigned long long() const { return static_cast<unsigned long long>(lo_); }

#ifdef NUMBERS_HAVE_INTRINSTIC_INT128
constexpr uint128::operator __int128() const { return (static_cast<__int128>(hi_) << 64) + lo_; }

constexpr uint128::operator unsigned __int128() const { return (static_cast<unsigned __int128>(hi_) << 64) + lo_; }
#endif

// Conversion operators to floating point types.

inline uint128::operator float() const { return static_cast<float>(lo_) + std::ldexp(static_cast<float>(hi_), 64); }

inline uint128::operator double() const { return static_cast<double>(lo_) + std::ldexp(static_cast<double>(hi_), 64); }

inline uint128::operator long double() const {
  return static_cast<long double>(lo_) + std::ldexp(static_cast<long double>(hi_), 64);
}

// comparison operators

constexpr bool operator==(uint128 lhs, uint128 rhs) {
#ifdef NUMBERS_HAVE_INTRINSTIC_INT128
  return static_cast<unsigned __int128>(lhs) == static_cast<unsigned __int128>(rhs);
#else
  return (uint128_low64(lhs) == uint128_low64(rhs)) && (uint128_high64(lhs) == uint128_high64(rhs));
#endif
}

constexpr bool operator!=(uint128 lhs, uint128 rhs) { return !(lhs == rhs); }

constexpr bool operator<(uint128 lhs, uint128 rhs) {
#ifdef NUMBERS_HAVE_INTRINSTIC_INT128
  return static_cast<unsigned __int128>(lhs) < static_cast<unsigned __int128>(rhs);
#else
  return (uint128_high64(lhs) == uint128_high64(rhs)) ? (uint128_low64(lhs) < uint128_low64(rhs))
                                                      : (uint128_high64(lhs) < uint128_high64(rhs));
#endif
}

constexpr bool operator>(uint128 lhs, uint128 rhs) { return rhs < lhs; }

constexpr bool operator<=(uint128 lhs, uint128 rhs) { return !(rhs < lhs); }

constexpr bool operator>=(uint128 lhs, uint128 rhs) { return !(lhs < rhs); }

// Unary operators

constexpr inline uint128 operator+(uint128 val) { return val; }

constexpr inline int128 operator+(int128 val) { return val; }

constexpr inline uint128 operator-(uint128 val) {
#ifdef NUMBERS_HAVE_INTRINSTIC_INT128
  return -static_cast<unsigned __int128>(val);
#else
  // The high 64 bits are the bitwise NOT of the high 64 bits of val plus 1 if the low 64 bits are zero
  return make_uint128(~uint128_high64(val) + static_cast<uint64_t>(uint128_low64(val) == 0), ~uint128_low64(val) + 1);
#endif
}

constexpr inline uint128 operator!(uint128 val) {
#ifdef NUMBERS_HAVE_INTRINSTIC_INT128
  return !static_cast<unsigned __int128>(val);
#else
  return !uint128_high64(val) && !uint128_low64(val);
#endif
}

constexpr inline uint128 operator~(uint128 val) {
#ifdef NUMBERS_HAVE_INTRINSTIC_INT128
  return ~static_cast<unsigned __int128>(val);
#else
  return make_uint128(~uint128_high64(val), ~uint128_low64(val));
#endif
}

constexpr inline uint128 operator|(uint128 lhs, uint128 rhs) {
#ifdef NUMBERS_HAVE_INTRINSTIC_INT128
  return static_cast<unsigned __int128>(lhs) | static_cast<unsigned __int128>(rhs);
#else
  return make_uint128(uint128_high64(lhs) | uint128_high64(rhs), uint128_low64(lhs) | uint128_low64(rhs));
#endif
}

constexpr inline uint128 operator&(uint128 lhs, uint128 rhs) {
#ifdef NUMBERS_HAVE_INTRINSTIC_INT128
  return static_cast<unsigned __int128>(lhs) & static_cast<unsigned __int128>(rhs);
#else
  return make_uint128(uint128_high64(lhs) & uint128_high64(rhs), uint128_low64(lhs) & uint128_low64(rhs));
#endif
}

constexpr inline uint128 operator^(uint128 lhs, uint128 rhs) {
#ifdef NUMBERS_HAVE_INTRINSTIC_INT128
  return static_cast<unsigned __int128>(lhs) ^ static_cast<unsigned __int128>(rhs);
#else
  return make_uint128(uint128_high64(lhs) ^ uint128_high64(rhs), uint128_low64(lhs) ^ uint128_low64(rhs));
#endif
}

inline uint128 &uint128::operator|=(uint128 other) {
  *this = *this | other;
  return *this;
}

inline uint128 &uint128::operator&=(uint128 other) {
  *this = *this & other;
  return *this;
}

inline uint128 &uint128::operator^=(uint128 other) {
  *this = *this ^ other;
  return *this;
}

constexpr uint128 operator<<(uint128 lhs, int amount) {
#ifdef NUMBERS_HAVE_INTRINSTIC_INT128
  return static_cast<unsigned __int128>(lhs) << amount;
#else
  return amount >= 64  ? make_uint128(uint128_low64(lhs) << (amount - 64), 0)
         : amount == 0 ? lhs
                       : make_uint128((uint128_high64(lhs) << amount) | (uint128_low64(lhs) >> (64 - amount)),
                                      uint128_low64(lhs) << amount);
#endif
}

constexpr uint128 operator>>(uint128 lhs, int amount) {
#ifdef NUMBERS_HAVE_INTRINSTIC_INT128
  return static_cast<unsigned __int128>(lhs) >> amount;
#else
  return amount >= 64  ? make_uint128(0, uint128_high64(lhs) >> (amount - 64))
         : amount == 0 ? lhs
                       : make_uint128(uint128_high64(lhs) >> amount,
                                      (uint128_low64(lhs) >> amount) | (uint128_high64(lhs) << (64 - amount)));
#endif
}

#ifndef NUMBERS_HAVE_INTRINSTIC_INT128
namespace int128_internal {
// Returns the result of adding two uint128 values, with overflow detected.
constexpr uint128 AddResult(uint128 result, uint128 lhs) {
  return (uint128_low64(result) < uint128_low64(lhs)) ? make_uint128(uint128_high64(result) + 1, uint128_low64(result))
                                                      : result;
}
}  // namespace int128_internal
#endif

constexpr uint128 operator+(uint128 lhs, uint128 rhs) {
#ifdef NUMBERS_HAVE_INTRINSTIC_INT128
  return static_cast<unsigned __int128>(lhs) + static_cast<unsigned __int128>(rhs);
#else
  return int128_internal::AddResult(
      make_uint128(uint128_high64(lhs) + uint128_high64(rhs), uint128_low64(lhs) + uint128_low64(rhs)), lhs);
#endif
}

#ifndef NUMBERS_HAVE_INTRINSTIC_INT128
namespace int128_internal {
constexpr uint128 SubtractResult(uint128 result, uint128 lhs, uint128 rhs) {
  return (uint128_low64(lhs) < uint128_low64(rhs)) ? make_uint128(uint128_high64(result) - 1, uint128_low64(result))
                                                   : result;
}
}  // namespace int128_internal
#endif

constexpr uint128 operator-(uint128 lhs, uint128 rhs) {
#ifdef NUMBERS_HAVE_INTRINSTIC_INT128
  return static_cast<unsigned __int128>(lhs) - static_cast<unsigned __int128>(rhs);
#else
  return int128_internal::SubtractResult(
      make_uint128(uint128_high64(lhs) - uint128_high64(rhs), uint128_low64(lhs) - uint128_low64(rhs)), lhs, rhs);
#endif
}

// Ref https://en.wikipedia.org/wiki/Karatsuba_algorithm
inline uint128 operator*(uint128 lhs, uint128 rhs) {
#ifdef NUMBERS_HAVE_INTRINSTIC_INT128
  return static_cast<unsigned __int128>(lhs) * static_cast<unsigned __int128>(rhs);
#elif defined(_MSC_VER) && defined(_M_X64) && !defined(_M_ARM64EC)
  uint64_t carry;
  uint64_t low = _umul128(uint128_low64(lhs), uint128_low64(rhs), &carry);
  return make_uint128(uint128_low64(lhs) * uint128_high64(rhs) + uint128_high64(lhs) * uint128_low64(rhs) + carry, low);
#else
  uint64_t a32 = uint128_low64(lhs) >> 32;
  uint64_t a00 = uint128_low64(lhs) & 0xffffffff;
  uint64_t b32 = uint128_low64(rhs) >> 32;
  uint64_t b00 = uint128_low64(rhs) & 0xffffffff;
  uint128 result = make_uint128(
      uint128_high64(lhs) * uint128_low64(rhs) + uint128_low64(lhs) * uint128_high64(rhs) + a32 * b32, a00 * b00);
  result += uint128(a32 * b00) << 32;
  result += uint128(a00 * b32) << 32;
  return result;
#endif
}

// Increment/decrement operators
inline uint128 uint128::operator++(int) {
  uint128 tmp(*this);
  *this += 1;
  return tmp;
}

inline uint128 uint128::operator--(int) {
  uint128 tmp(*this);
  *this -= 1;
  return tmp;
}

inline uint128 &uint128::operator++() {
  *this += 1;
  return *this;
}

inline uint128 &uint128::operator--() {
  *this -= 1;
  return *this;
}

constexpr int128 make_int128(int64_t high, uint64_t low) { return int128(high, low); }

// Assignments from integer types.
inline int128 &int128::operator=(int v) { return *this = int128(v); }

inline int128 &int128::operator=(unsigned int v) { return *this = int128(v); }

inline int128 &int128::operator=(long v) { return *this = int128(v); }

inline int128 &int128::operator=(unsigned long v) { return *this = int128(v); }

inline int128 &int128::operator=(long long v) { return *this = int128(v); }

inline int128 &int128::operator=(unsigned long long v) { return *this = int128(v); }

// Arithmetic operators.
constexpr int128 operator-(int128 v);
constexpr int128 operator+(int128 lhs, int128 rhs);
constexpr int128 operator-(int128 lhs, int128 rhs);
int128 operator*(int128 lhs, int128 rhs);
int128 operator/(int128 lhs, int128 rhs);
int128 operator%(int128 lhs, int128 rhs);
constexpr int128 operator|(int128 lhs, int128 rhs);
constexpr int128 operator&(int128 lhs, int128 rhs);
constexpr int128 operator^(int128 lhs, int128 rhs);
constexpr int128 operator<<(int128 lhs, int amount);
constexpr int128 operator>>(int128 lhs, int amount);

inline int128 &int128::operator+=(int128 other) {
  *this = *this + other;
  return *this;
}

inline int128 &int128::operator-=(int128 other) {
  *this = *this - other;
  return *this;
}

inline int128 &int128::operator*=(int128 other) {
  *this = *this * other;
  return *this;
}

inline int128 &int128::operator/=(int128 other) {
  *this = *this / other;
  return *this;
}

inline int128 &int128::operator%=(int128 other) {
  *this = *this % other;
  return *this;
}

inline int128 &int128::operator|=(int128 other) {
  *this = *this | other;
  return *this;
}

inline int128 &int128::operator&=(int128 other) {
  *this = *this & other;
  return *this;
}

inline int128 &int128::operator^=(int128 other) {
  *this = *this ^ other;
  return *this;
}

inline int128 &int128::operator<<=(int amount) {
  *this = *this << amount;
  return *this;
}

inline int128 &int128::operator>>=(int amount) {
  *this = *this >> amount;
  return *this;
}

// Forward declarations for comparison operators
constexpr bool operator!=(int128 lhs, int128 rhs);

// Casts from unsigned to signed while preserving the underlying binary representation.
namespace int128_internal {
constexpr int64_t BitCastToSigned(uint64_t v) {
  return v & (uint64_t{1} << 63) ? ~static_cast<int64_t>(~v) : static_cast<int64_t>(v);
}
}  // namespace int128_internal

#ifdef NUMBERS_HAVE_INTRINSTIC_INT128
#include "int128_have_intrinstic.inc"
#else
#include "int128_no_intrinstic.inc"
#endif

}  // namespace numbers

#endif