#ifndef HEADER_I128_HH
#define HEADER_I128_HH
#include <cstdint>

#include "uinteger.hh"

class Int128 {
 private:
  constexpr Int128(Uint64 high, Uint64 low) : low_(low), high_(high) {}

 public:
  constexpr Int128(uint64_t num) : Int128(0, num) {}

  constexpr Int128 operator+(const Int128 &other) const noexcept {
    auto [low, flag] = this->low_.overflowing_add(other.low_);
    auto high = this->high_ + other.high_ + flag;
    return Int128(high, low);
  }

  constexpr bool operator==(const Int128 &other) const noexcept {
    return this->low_ == other.low_ && this->high_ == other.high_;
  }

 private:
  Uint64 low_;
  Uint64 high_;
};

constexpr Int128 operator"" _i128(unsigned long long val) { return Int128(val); }

#endif