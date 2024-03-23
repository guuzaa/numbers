namespace int128_internal {
constexpr __int128 BitCastToSigned(unsigned __int128 v) {
  return (v & (static_cast<unsigned __int128>(1) << 127)) ? ~static_cast<__int128>(~v) : static_cast<__int128>(v);
}
}  // namespace int128_internal

inline int128 &int128::operator=(__int128 v) {
  v_ = v;
  return *this;
}

constexpr uint64_t int128_low64(int128 v) { return static_cast<uint64_t>(v.v_ & ~uint64_t{0}); }

constexpr int64_t int128_high64(int128 v) {
  return int128_internal::BitCastToSigned(static_cast<uint64_t>(static_cast<unsigned __int128>(v.v_) >> 64));
}

constexpr int128::int128(int64_t high, uint64_t low)
    : v_(int128_internal::BitCastToSigned(static_cast<unsigned __int128>(high) << 64) | low) {}

constexpr int128::int128(int v) : v_{v} {}
constexpr int128::int128(long v) : v_{v} {}
constexpr int128::int128(long long v) : v_{v} {}
constexpr int128::int128(__int128 v) : v_{v} {}
constexpr int128::int128(unsigned int v) : v_{v} {}
constexpr int128::int128(unsigned long v) : v_{v} {}
constexpr int128::int128(unsigned long long v) : v_{v} {}
constexpr int128::int128(unsigned __int128 v) : v_{static_cast<__int128>(v)} {}

inline int128::int128(float v) { v_ = static_cast<__int128>(v); }
inline int128::int128(double v) { v_ = static_cast<__int128>(v); }
inline int128::int128(long double v) { v_ = static_cast<__int128>(v); }

constexpr int128::int128(uint128 v) : v_{static_cast<__int128>(v)} {}

constexpr int128::operator bool() const { return static_cast<bool>(v_); }
constexpr int128::operator char() const { return static_cast<char>(v_); }
constexpr int128::operator signed char() const { return static_cast<signed char>(v_); }
constexpr int128::operator unsigned char() const { return static_cast<unsigned char>(v_); }
constexpr int128::operator char16_t() const { return static_cast<char16_t>(v_); }
constexpr int128::operator char32_t() const { return static_cast<char32_t>(v_); }

constexpr int128::operator short() const { return static_cast<short>(v_); }
constexpr int128::operator unsigned short() const { return static_cast<unsigned short>(v_); }
constexpr int128::operator int() const { return static_cast<int>(v_); }
constexpr int128::operator unsigned int() const { return static_cast<unsigned int>(v_); }
constexpr int128::operator long() const { return static_cast<long>(v_); }
constexpr int128::operator unsigned long() const { return static_cast<unsigned long>(v_); }
constexpr int128::operator long long() const { return static_cast<long long>(v_); }
constexpr int128::operator unsigned long long() const { return static_cast<unsigned long long>(v_); }

constexpr int128::operator __int128() const { return static_cast<__int128>(v_); }
constexpr int128::operator unsigned __int128() const { return static_cast<unsigned __int128>(v_); }

// Clang on PowerPC sometimes produces incorrect __int128 to floating point
// conversions.
#if defined(__clang__) && !defined(__ppc64__)
inline int128::operator float() const { return static_cast<float>(v_); }

inline int128::operator double() const { return static_cast<double>(v_); }

inline int128::operator long double() const { return static_cast<long double>(v_); }

#else   // Clang on PowerPC

inline int128::operator float() const {
  // We must convert the absolute value and then negate as needed, because
  // floating point types are typically sign-magnitude. Otherwise, the
  // difference between the high and low 64 bits when interpreted as two's
  // complement overwhelms the precision of the mantissa.
  //
  // Also check to make sure we don't negate Int128Min()
  return v_ < 0 && *this != MIN
             ? -static_cast<float>(-*this)
             : static_cast<float>(int128_low64(*this)) + std::ldexp(static_cast<float>(int128_high64(*this)), 64);
}

inline int128::operator double() const {
  // See comment in int128::operator float() above.
  return v_ < 0 && *this != MIN
             ? -static_cast<double>(-*this)
             : static_cast<double>(int128_low64(*this)) + std::ldexp(static_cast<double>(int128_high64(*this)), 64);
}

inline int128::operator long double() const {
  // See comment in int128::operator float() above.
  return v_ < 0 && *this != MIN ? -static_cast<long double>(-*this)
                                : static_cast<long double>(int128_low64(*this)) +
                                      std::ldexp(static_cast<long double>(int128_high64(*this)), 64);
}
#endif  // Clang on PowerPC

// Comparison operators.

constexpr bool operator==(int128 lhs, int128 rhs) { return static_cast<__int128>(lhs) == static_cast<__int128>(rhs); }

constexpr bool operator!=(int128 lhs, int128 rhs) { return static_cast<__int128>(lhs) != static_cast<__int128>(rhs); }

constexpr bool operator<(int128 lhs, int128 rhs) { return static_cast<__int128>(lhs) < static_cast<__int128>(rhs); }

constexpr bool operator>(int128 lhs, int128 rhs) { return static_cast<__int128>(lhs) > static_cast<__int128>(rhs); }

constexpr bool operator<=(int128 lhs, int128 rhs) { return static_cast<__int128>(lhs) <= static_cast<__int128>(rhs); }

constexpr bool operator>=(int128 lhs, int128 rhs) { return static_cast<__int128>(lhs) >= static_cast<__int128>(rhs); }

// Unary operators.

constexpr int128 operator-(int128 v) { return -static_cast<__int128>(v); }

constexpr bool operator!(int128 v) { return !static_cast<__int128>(v); }

constexpr int128 operator~(int128 val) { return ~static_cast<__int128>(val); }

// Arithmetic operators.

constexpr int128 operator+(int128 lhs, int128 rhs) { return static_cast<__int128>(lhs) + static_cast<__int128>(rhs); }

constexpr int128 operator-(int128 lhs, int128 rhs) { return static_cast<__int128>(lhs) - static_cast<__int128>(rhs); }

inline int128 operator*(int128 lhs, int128 rhs) { return static_cast<__int128>(lhs) * static_cast<__int128>(rhs); }

inline int128 operator/(int128 lhs, int128 rhs) { return static_cast<__int128>(lhs) / static_cast<__int128>(rhs); }

inline int128 operator%(int128 lhs, int128 rhs) { return static_cast<__int128>(lhs) % static_cast<__int128>(rhs); }

inline int128 int128::operator++(int) {
  int128 tmp(*this);
  ++v_;
  return tmp;
}

inline int128 int128::operator--(int) {
  int128 tmp(*this);
  --v_;
  return tmp;
}

inline int128 &int128::operator++() {
  ++v_;
  return *this;
}

inline int128 &int128::operator--() {
  --v_;
  return *this;
}

constexpr int128 operator|(int128 lhs, int128 rhs) { return static_cast<__int128>(lhs) | static_cast<__int128>(rhs); }

constexpr int128 operator&(int128 lhs, int128 rhs) { return static_cast<__int128>(lhs) & static_cast<__int128>(rhs); }

constexpr int128 operator^(int128 lhs, int128 rhs) { return static_cast<__int128>(lhs) ^ static_cast<__int128>(rhs); }

constexpr int128 operator<<(int128 lhs, int amount) { return static_cast<__int128>(lhs) << amount; }

constexpr int128 operator>>(int128 lhs, int amount) { return static_cast<__int128>(lhs) >> amount; }
