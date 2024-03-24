constexpr uint64_t int128_low64(int128 v) { return v.lo_; }

constexpr int64_t int128_high64(int128 v) { return v.hi_; }

constexpr int128::int128(int64_t high, uint64_t low) : lo_{low}, hi_{high} {}

constexpr int128::int128(int v) : lo_{static_cast<uint64_t>(v)}, hi_{v < 0 ? ~int64_t{0} : 0} {}

constexpr int128::int128(long v) : lo_{static_cast<uint64_t>(v)}, hi_{v < 0 ? ~int64_t{0} : 0} {}

constexpr int128::int128(long long v) : lo_{static_cast<uint64_t>(v)}, hi_{v < 0 ? ~int64_t{0} : 0} {}

constexpr int128::int128(unsigned int v) : lo_{v}, hi_{0} {}

constexpr int128::int128(unsigned long v) : lo_{v}, hi_{0} {}

constexpr int128::int128(unsigned long long v) : lo_{v}, hi_{0} {}

constexpr int128::int128(uint128 v) : lo_{uint128_low64(v)}, hi_{static_cast<int64_t>(uint128_high64(v))} {}

constexpr int128::operator bool() const { return lo_ || hi_; }

constexpr int128::operator char() const { return static_cast<char>(static_cast<long long>(*this)); }

constexpr int128::operator signed char() const { return static_cast<signed char>(static_cast<long long>(*this)); }

constexpr int128::operator unsigned char() const { return static_cast<unsigned char>(lo_); }

constexpr int128::operator char16_t() const { return static_cast<char16_t>(lo_); }

constexpr int128::operator char32_t() const { return static_cast<char32_t>(lo_); }

constexpr int128::operator short() const { return static_cast<short>(static_cast<long long>(*this)); }

constexpr int128::operator unsigned short() const { return static_cast<unsigned short>(lo_); }

constexpr int128::operator int() const { return static_cast<int>(static_cast<long long>(*this)); }

constexpr int128::operator unsigned int() const { return static_cast<unsigned int>(lo_); }

constexpr int128::operator long() const { return static_cast<long>(static_cast<long long>(*this)); }

constexpr int128::operator unsigned long() const { return static_cast<unsigned long>(lo_); }

constexpr int128::operator long long() const { return int128_internal::BitCastToSigned(lo_); }

constexpr int128::operator unsigned long long() const { return static_cast<unsigned long long>(lo_); }

inline int128::operator float() const {
  // We must convert the absolute value and then negate as needed, because
  // floating point types are typically sign-magnitude. Otherwise, the
  // difference between the high and low 64 bits when interpreted as two's
  // complement overwhelms the precision of the mantissa.
  //
  // Also check to make sure we don't negate Int128Min()
  return v_ < 0 && *this != MIN ? -static_cast<float>(-*this)
                                : static_cast<float>(lo_) + std::ldexp(static_cast<float>(hi_), 64);
}

inline int128::operator double() const {
  // See comment in int128::operator float() above.
  return v_ < 0 && *this != MIN ? -static_cast<double>(-*this)
                                : static_cast<double>(lo_) + std::ldexp(static_cast<double>(hi_), 64);
}

inline int128::operator long double() const {
  // See comment in int128::operator float() above.
  return v_ < 0 && *this != MIN ? -static_cast<long double>(-*this)
                                : static_cast<long double>(lo_) + std::ldexp(static_cast<long double>(hi_), 64);
}

// Comparison operators
constexpr bool operator==(int128 lhs, int128 rhs) {
  return (int128_low64(lhs) == int128_low64(rhs) && int128_high64(lhs) == int128_high64(rhs));
}

constexpr bool operator!=(int128 lhs, int128 rhs) { return !(lhs == rhs); }

constexpr bool operator<(int128 lhs, int128 rhs) {
  return (int128_high64(lhs) == int128_high64(rhs)) ? (int128_low64(lhs) < int128_low64(rhs))
                                                    : (int128_high64(lhs) < int128_high64(rhs));
}

constexpr bool operator>(int128 lhs, int128 rhs) {
  return (int128_high64(lhs) == int128_high64(rhs)) ? (int128_low64(lhs) > int128_low64(rhs))
                                                    : (int128_high64(lhs) > int128_high64(rhs));
}

constexpr bool operator<=(int128 lhs, int128 rhs) { return !(lhs > rhs); }

constexpr bool operator>=(int128 lhs, int128 rhs) { return !(lhs < rhs); }

// Unary operators
constexpr int128 operator-(int128 v) {
  return make_int128(~int128_high64(v) + (int128_low64(v) == 0), ~int128_low64(v) + 1);
}

constexpr int128 operator!(int128 v) { return !int128_high64(v) && !int128_low64(v); }

constexpr int128 operator~(int128 v) { return make_int128(~int128_high64(v), ~int128_low64(v)); }

// Arithmetic operators
namespace int128_internal {
constexpr int128 SignedAddResult(int128 result, int128 lhs) {
  // check for carry
  return (int128_low64(result) < int128_low64(lhs)) ? make_int128(int128_high64(result) + 1, int128_low64(result))
                                                    : result;
}
}  // namespace int128_internal

constexpr int128 operator+(int128 lhs, int128 rhs) {
  return int128_internal::SignedAddResult(
      make_int128(int128_high64(lhs) + int128_high64(rhs), int128_low64(lhs) + int128_low64(rhs)), lhs);
}

namespace int128_internal {
constexpr int128 SignedSubstructResult(int128 result, int128 lhs, int128 rhs) {
  // check for carry
  return (int128_low64(lhs) < int128_low64(rhs)) ? make_int128(int128_high64(result) - 1, int128_low64(result))
                                                 : result;
}
}  // namespace int128_internal

constexpr int128 operator-(int128 lhs, int128 rhs) {
  return int128_internal::SignedSubstructResult(
      make_int128(int128_high64(lhs) - int128_high64(rhs), int128_low64(lhs) - int128_low64(rhs)), lhs, rhs);
}

inline int128 operator*(int128 lhs, int128 rhs) {
  return make_int128(int128_internal::BitCastToSigned(uint128_high64(uint128(lhs) * uint128(rhs))),
                     uint128_low64(uint128(lhs) * uint128(rhs)));
}

inline int128 int128::operator++(int) {
  int128 tmp(*this);
  *this += 1;
  return tmp;
}

inline int128 int128::operator--(int) {
  int128 tmp(*this);
  *this -= 1;
  return tmp;
}

inline int128 &int128::operator++() {
  *this += 1;
  return *this;
}

inline int128 &int128::operator--() {
  *this -= 1;
  return *this;
}

constexpr int128 operator|(int128 lhs, int128 rhs) {
  return make_int128(int128_high64(lhs) | int128_high64(rhs), int128_low64(lhs) | int128_low64(rhs));
}

constexpr int128 operator&(int128 lhs, int128 rhs) {
  return make_int128(int128_high64(lhs) & int128_high64(rhs), int128_low64(lhs) & int128_low64(rhs));
}

constexpr int128 operator^(int128 lhs, int128 rhs) {
  return make_int128(int128_high64(lhs) ^ int128_high64(rhs), int128_low64(lhs) ^ int128_low64(rhs));
}

constexpr int128 operator<<(int128 lhs, int amount) {
  // int64_t shifts of >= 63 are undefined, so we need some special-casing.
  assert(amount >= 0 && amount < 127);

  if (amount <= 0) {
    return lhs;
  } else if (amount < 63) {
    return make_int128((int128_high64(lhs) << amount) | static_cast<int64_t>(int128_low64(lhs) >> (64 - amount)),
                       int128_low64(lhs) << amount);
  } else if (amount == 63) {
    return make_int128(((int128_high64(lhs) << 32) << 31) | static_cast<int64_t>(int128_low64(lhs) >> 1),
                       (int128_low64(lhs) << 32) << 31);
  } else if (amount == 127) {
    return make_int128(static_cast<int64_t>(int128_low64(lhs) << 63), 0);
  } else if (amount > 127) {
    return make_int128(0, 0);
  } else {  // amount >= 64 && amount < 127
    return make_int128(static_cast<int64_t>(int128_low64(lhs) << (amount - 64)), 0);
  }
}

constexpr int128 operator>>(int128 lhs, int amount) {
  // int64_t shifts of >= 63 are undefined, so we need some special-casing.
  assert(amount >= 0 && amount < 127);

  if (amount <= 0) {
    return lhs;
  } else if (amount < 63) {
    return make_int128(int128_high64(lhs) >> amount,
                       int128_low64(lhs) >> amount | static_cast<uint64_t>(int128_high64(lhs)) << (64 - amount));
  } else if (amount == 63) {
    return make_int128((int128_high64(lhs) >> 32) >> 31,
                       static_cast<uint64_t>(int128_high64(lhs) << 1) | (int128_low64(lhs) >> 32) >> 31);
  } else if (amount >= 127) {
    return make_int128((int128_high64(lhs) >> 32) >> 31, static_cast<uint64_t>((int128_high64(lhs) >> 32) >> 31));
  } else {  // amount >= 64 && amount < 127
    return make_int128((int128_high64(lhs) >> 32) >> 31, static_cast<uint64_t>(int128_high64(lhs) >> (amount - 64)));
  }
}