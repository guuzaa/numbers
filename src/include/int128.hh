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
// modified from abseil-cpp

#ifndef NUMBERS_INT128_HH
#define NUMBERS_INT128_HH

#include <cmath>
#include <string>

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

  // Uint128Low64()
  //
  // Returns the lower 64-bit value of a `uint128` value.
  friend constexpr uint64_t Uint128Low64(uint128 v);

  // Uint128High64()
  //
  // Returns the higher 64-bit value of a `uint128` value.
  friend constexpr uint64_t Uint128High64(uint128 v);

  // MakeUInt128()
  //
  // Constructs a `uint128` numeric value from two 64-bit unsigned integers.
  // Note that this factory function is the only way to construct a `uint128`
  // from integer values greater than 2^64.
  //
  // Example:
  //
  //   numbers::uint128 big = numbers::MakeUint128(1, 0);
  friend constexpr uint128 MakeUint128(uint64_t high, uint64_t low);

  // Uint128Max()
  //
  // Returns the highest value for a 128-bit unsigned integer.
  friend constexpr uint128 Uint128Max();

 private:
  constexpr uint128(uint64_t high, uint64_t low);

  std::string ToString() const;

  // little endian
  uint64_t lo_;
  uint64_t hi_;
};

std::ostream &operator<<(std::ostream &os, uint128 v);

constexpr uint128 Uint128Max() {
  return uint128((std::numeric_limits<uint64_t>::max)(), (std::numeric_limits<uint64_t>::max)());
}
}  // namespace numbers

// Specialized numeric_limits for uint128.
namespace std {
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
  static constexpr bool traps = numeric_limits<uint64_t>::traps;
  static constexpr bool tinyness_before = false;

  static constexpr numbers::uint128(min)() { return 0; }
  static constexpr numbers::uint128 lowest() { return 0; }
  static constexpr numbers::uint128(max)() { return numbers::Uint128Max(); }
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

  // Int128Low64()
  //
  // Returns the lower 64-bit value of a `int128` value.
  friend constexpr uint64_t Int128Low64(int128 v);

  // Int128High64()
  //
  // Returns the higher 64-bit value of a `int128` value.
  friend constexpr int64_t Int128High64(int128 v);

  // MakeInt128()
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
  //   numbers::int128 big = numbers::MakeInt128(1, 0);
  //   numbers::int128 big_n = numbers::MakeInt128(-1, 0);
  friend constexpr int128 MakeInt128(int64_t high, uint64_t low);

  // Int128Max()
  //
  // Returns the maximum value for a 128-bit signed integer.
  friend constexpr int128 Int128Max();

  // Int128Min()
  //
  // Returns the minimum value for a 128-bit signed integer.
  friend constexpr int128 Int128Min();

 private:
  constexpr int128(int64_t high, uint64_t low);

  std::string ToString() const;

  // little endian
  uint64_t lo_;
  int64_t hi_;
};

std::ostream &operator<<(std::ostream &os, int128 v);

constexpr int128 Int128Max() {
  return int128((std::numeric_limits<int64_t>::max)(), (std::numeric_limits<uint64_t>::max)());
}

constexpr int128 Int128Min() { return int128((std::numeric_limits<int64_t>::min)(), 0); }

}  // namespace numbers

namespace std {
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
  static constexpr bool traps = numeric_limits<uint64_t>::traps;
  static constexpr bool tinyness_before = false;

  static constexpr numbers::int128(min)() { return numbers::Int128Min(); }
  static constexpr numbers::int128 lowest() { return numbers::Int128Min(); }
  static constexpr numbers::int128(max)() { return numbers::Int128Max(); }
  static constexpr numbers::int128 epsilon() { return 0; }
  static constexpr numbers::int128 round_error() { return 0; }
  static constexpr numbers::int128 infinity() { return 0; }
  static constexpr numbers::int128 quiet_NaN() { return 0; }
  static constexpr numbers::int128 signaling_NaN() { return 0; }
  static constexpr numbers::int128 denorm_min() { return 0; }
};
}  // namespace std

namespace numbers {
constexpr uint128 MakeUint128(uint64_t high, uint64_t low) { return uint128(high, low); }

// Assignments from integer types.

inline uint128 &uint128::operator=(int v) { return *this = uint128(v); }

inline uint128 &uint128::operator=(unsigned int v) { return *this = uint128(v); }

inline uint128 &uint128::operator=(long v) { return *this = uint128(v); }

inline uint128 &uint128::operator=(unsigned long v) { return *this = uint128(v); }

inline uint128 &uint128::operator=(long long v) { return *this = uint128(v); }

inline uint128 &uint128::operator=(unsigned long long v) { return *this = uint128(v); }

inline uint128 &uint128::operator=(int128 v) { return *this = uint128(v); }

// Arithmetic operators.

constexpr uint128 operator<<(uint128 lhs, int amout);
constexpr uint128 operator>>(uint128 lhs, int amout);
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

constexpr uint64_t Uint128Low64(uint128 v) { return v.lo_; }

constexpr uint64_t Uint128High64(uint128 v) { return v.hi_; }

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

constexpr uint128::uint128(int128 v) : lo_{Int128Low64(v)}, hi_{static_cast<uint64_t>(Int128High64(v))} {}

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

// Conversion operators to floating point types.

inline uint128::operator float() const { return static_cast<float>(lo_) + std::ldexp(static_cast<float>(hi_), 64); }

inline uint128::operator double() const { return static_cast<double>(lo_) + std::ldexp(static_cast<double>(hi_), 64); }

inline uint128::operator long double() const {
  return static_cast<long double>(lo_) + std::ldexp(static_cast<long double>(hi_), 64);
}

// comparison operators

constexpr bool operator==(uint128 lhs, uint128 rhs) {
  return (Uint128Low64(lhs) == Uint128Low64(rhs)) && (Uint128High64(lhs) == Uint128High64(rhs));
}

constexpr bool operator!=(uint128 lhs, uint128 rhs) { return !(lhs == rhs); }

constexpr bool operator<(uint128 lhs, uint128 rhs) {
  return (Uint128High64(lhs) == Uint128High64(rhs)) ? (Uint128Low64(lhs) < Uint128Low64(rhs))
                                                    : (Uint128High64(lhs) < Uint128High64(rhs));
}

constexpr bool operator>(uint128 lhs, uint128 rhs) { return rhs < lhs; }

constexpr bool operator<=(uint128 lhs, uint128 rhs) { return !(rhs < lhs); }

constexpr bool operator>=(uint128 lhs, uint128 rhs) { return !(lhs < rhs); }

// Unary operators

constexpr inline uint128 operator+(uint128 val) { return val; }

constexpr inline int128 operator+(int128 val) { return val; }

constexpr inline uint128 operator-(uint128 val) {
  // The high 64 bits are the bitwise NOT of the high 64 bits of val plus 1 if the low 64 bits are zero
  return MakeUint128(~Uint128High64(val) + static_cast<uint64_t>(Uint128Low64(val) == 0), ~Uint128Low64(val) + 1);
}

constexpr inline uint128 operator!(uint128 val) { return !Uint128High64(val) && !Uint128Low64(val); }

constexpr inline uint128 operator~(uint128 val) { return MakeUint128(~Uint128High64(val), ~Uint128Low64(val)); }

constexpr inline uint128 operator|(uint128 lhs, uint128 rhs) {
  return MakeUint128(Uint128High64(lhs) | Uint128High64(rhs), Uint128Low64(lhs) | Uint128Low64(rhs));
}

constexpr inline uint128 operator&(uint128 lhs, uint128 rhs) {
  return MakeUint128(Uint128High64(lhs) & Uint128High64(rhs), Uint128Low64(lhs) & Uint128Low64(rhs));
}

constexpr inline uint128 operator^(uint128 lhs, uint128 rhs) {
  return MakeUint128(Uint128High64(lhs) ^ Uint128High64(rhs), Uint128Low64(lhs) ^ Uint128Low64(rhs));
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
  return amount >= 64  ? MakeUint128(Uint128Low64(lhs) << (amount - 64), 0)
         : amount == 0 ? lhs
                       : MakeUint128((Uint128High64(lhs) << amount) | (Uint128Low64(lhs) >> (64 - amount)),
                                     Uint128Low64(lhs) << amount);
}

constexpr uint128 operator>>(uint128 lhs, int amount) {
  return amount >= 64  ? MakeUint128(0, Uint128High64(lhs) >> (amount - 64))
         : amount == 0 ? lhs
                       : MakeUint128(Uint128High64(lhs) >> amount,
                                     (Uint128Low64(lhs) >> amount) | (Uint128High64(lhs) << (64 - amount)));
}

namespace int128_internal {
  // Returns the result of adding two uint128 values, with overflow detected.
constexpr uint128 AddResult(uint128 result, uint128 lhs) {
  return (Uint128Low64(result) < Uint128Low64(lhs)) ? MakeUint128(Uint128High64(result) + 1, Uint128Low64(result))
                                                    : result;
}
}  // namespace int128_internal

constexpr uint128 operator+(uint128 lhs, uint128 rhs) {
  return int128_internal::AddResult(
      MakeUint128(Uint128High64(lhs) + Uint128High64(rhs), Uint128Low64(lhs) + Uint128Low64(rhs)), lhs);
}

}  // namespace numbers

#endif