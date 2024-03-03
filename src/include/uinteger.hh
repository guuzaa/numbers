#ifndef HEADER_UNINTEGER_H
#define HEADER_UNINTEGER_H

#include <iostream>
#include <limits>
#include <optional>
#include <type_traits>
#include "int128.hh"

namespace numbers {

template <typename T, typename = std::enable_if_t<std::is_unsigned_v<T> || std::is_same_v<T, uint128>>>
class Uinteger {
 private:
  constexpr static T min_ = std::numeric_limits<T>::min();
  constexpr static T max_ = std::numeric_limits<T>::max();

 public:
  inline static Uinteger<T> MIN = Uinteger(min_);
  inline static Uinteger<T> MAX = Uinteger(max_);

  constexpr Uinteger() noexcept : num_{} {}
  template <typename U, typename = std::enable_if<std::is_convertible_v<U, T> && std::is_integral_v<U>>>
  Uinteger(U num) noexcept : num_{static_cast<T>(num)} {}

  constexpr Uinteger operator+(const Uinteger<T> &other) const noexcept(false) {
    if (add_overflow(num_, other.num_)) {
      throw std::runtime_error("add overflow");
    }
    return Uinteger(num_ + other.num_);
  }

  constexpr Uinteger wrapping_add(const Uinteger<T> &other) const noexcept { return Uinteger(num_ + other.num_); }

  constexpr std::optional<Uinteger> checked_add(const Uinteger<T> &other) const noexcept {
    if (add_overflow(num_, other.num_)) {
      return {};
    }
    return Uinteger(num_ + other.num_);
  }

  constexpr std::tuple<Uinteger<T>, bool> overflowing_add(const Uinteger<T> &other) const noexcept {
    return {Uinteger(num_ + other.num_), add_overflow(num_, other.num_)};
  }

  constexpr Uinteger<T> saturating_add(const Uinteger<T> &other) const noexcept {
    if (add_overflow(num_, other.num_)) {
      return Uinteger(MAX);
    }
    return Uinteger(num_ + other.num_);
  }

  constexpr Uinteger operator-(const Uinteger<T> &other) const noexcept(false) {
    if (sub_overflow(num_, other.num_)) {
      throw std::runtime_error("sub overflow");
    }
    return Uinteger(num_ - other.num_);
  }

  constexpr Uinteger wrapping_sub(const Uinteger<T> &other) const noexcept { return Uinteger(num_ - other.num_); }

  constexpr std::optional<Uinteger> checked_sub(const Uinteger<T> &other) const noexcept {
    if (sub_overflow(num_, other.num_)) {
      return {};
    }
    return Uinteger(num_ - other.num_);
  }

  constexpr std::tuple<Uinteger<T>, bool> overflowing_sub(const Uinteger<T> &other) const noexcept {
    return {Uinteger(num_ - other.num_), sub_overflow(num_, other.num_)};
  }

  constexpr Uinteger saturating_sub(const Uinteger<T> &other) const noexcept {
    if (sub_overflow(num_, other.num_)) {
      return MIN;
    }
    return Uinteger(num_ - other.num_);
  }

  constexpr Uinteger operator/(const Uinteger<T> &other) const noexcept(false) {
    if (div_overflow(num_, other.num_)) {
      throw std::runtime_error("div overflow");
    }
    return Uinteger(num_ / other.num_);
  }

  constexpr Uinteger wrapping_div(const Uinteger<T> &other) const noexcept { return Uinteger(num_ / other.num_); }

  constexpr std::optional<Uinteger> checked_div(const Uinteger<T> &other) const noexcept {
    if (div_overflow(num_, other.num_)) {
      return {};
    }
    return Uinteger(num_ / other.num_);
  }

  constexpr std::tuple<Uinteger<T>, bool> overflowing_div(const Uinteger<T> &other) const noexcept {
    return {Uinteger(num_ / other.num_), div_overflow(num_, other.num_)};
  }

  constexpr Uinteger saturating_div(const Uinteger<T> &other) const noexcept {
    if (div_overflow(num_, other.num_)) {
      return MIN;
    }
    return Uinteger(num_ / other.num_);
  }

  constexpr Uinteger operator*(const Uinteger<T> &other) const noexcept(false) {
    if (mul_overflow(num_, other.num_)) {
      throw std::runtime_error("mul overflow");
    }
    return Uinteger(num_ * other.num_);
  }

  constexpr Uinteger wrapping_mul(const Uinteger<T> &other) const noexcept { return Uinteger(num_ * other.num_); }

  constexpr std::optional<Uinteger> checked_mul(const Uinteger<T> &other) const noexcept {
    if (mul_overflow(num_, other.num_)) {
      return {};
    }
    return Uinteger(num_ * other.num_);
  }

  constexpr std::tuple<Uinteger<T>, bool> overflowing_mul(const Uinteger<T> &other) const noexcept {
    return {Uinteger(num_ * other.num_), mul_overflow(num_, other.num_)};
  }

  constexpr Uinteger saturating_mul(const Uinteger<T> &other) const noexcept {
    if (mul_overflow(num_, other.num_)) {
      return MAX;
    }
    return Uinteger(num_ * other.num_);
  }

  constexpr Uinteger operator-() const noexcept(false) {
    if (num_ == min_) {
      return Uinteger(num_);
    }
    throw std::runtime_error("neg overflow");
  }

  constexpr std::optional<Uinteger> checked_neg() const noexcept {
    if (num_ == min_) {
      return MIN;
    }
    return {};
  }

  constexpr std::tuple<Uinteger, bool> overflowing_neg() const noexcept {
    if (num_ == min_) {
      return {MIN, false};
    }
    return {Uinteger(-num_), true};
  }

  constexpr Uinteger wrapping_neg() const noexcept {
    if (num_ == min_) {
      return MIN;
    }
    return Uinteger(-num_);
  }

  constexpr bool operator==(Uinteger<T> other) const noexcept { return num_ == other.num_; }
  constexpr bool operator<(Uinteger<T> other) const noexcept { return num_ < other.num_; }
  constexpr bool operator>(Uinteger<T> other) const noexcept { return num_ > other.num_; }

  Uinteger &operator+=(Uinteger other) {
    *this = *this + other;
    return *this;
  }

  Uinteger &operator-=(Uinteger other) {
    *this = *this - other;
    return *this;
  }

  Uinteger &operator/=(Uinteger other) {
    *this = *this / other;
    return *this;
  }

  Uinteger &operator*=(Uinteger other) {
    *this = *this * other;
    return *this;
  }

  Uinteger operator<<(int amount) {
    num_ <<= amount;
    return *this;
  }

  Uinteger operator>>(int amount) {
    num_ >>= amount;
    return *this;
  }

  Uinteger &operator<<=(int amount) {
    *this <<= amount;
    return *this;
  }

  Uinteger &operator>>=(int amount) {
    *this >>= amount;
    return *this;
  }

  // prefix ++
  constexpr Uinteger &operator++() noexcept(false) {
    *this += 1;
    return *this;
  }

  // postfix ++
  constexpr Uinteger operator++(int) noexcept(false) {
    Uinteger<T> tmp = *this;
    *this += 1;
    return tmp;
  }

  // prefix --
  constexpr Uinteger &operator--() noexcept(false) {
    *this -= 1;
    return *this;
  }

  // postfix ++
  constexpr Uinteger operator--(int) noexcept(false) {
    Uinteger<T> tmp = *this;
    *this -= 1;
    return tmp;
  }

  template <typename U, typename = std::enable_if<std::is_convertible_v<T, U>>>
  explicit operator Uinteger<U>() const {
    return Uinteger<U>(static_cast<U>(num_));
  }

  template <typename U, typename = std::enable_if<std::is_convertible_v<U, T>>>
  explicit operator U() const noexcept {
    return static_cast<U>(num_);
  }

  friend std::ostream &operator<<(std::ostream &os, const Uinteger<T> &num) {
    os << num.num_;
    return os;
  }

 private:
  constexpr bool add_overflow(T a, T b) const noexcept { return b > max_ - a; }

  constexpr bool sub_overflow(T minuend, T subtrahend) const noexcept { return minuend < subtrahend; }

  constexpr bool div_overflow(T a, T b) const noexcept { return false; }

  constexpr bool mul_overflow(T a, T b) const {
    T res;
    return __builtin_mul_overflow(a, b, &res);
  }

  T num_;
};

template <typename T>
constexpr Uinteger<T> &operator+=(Uinteger<T> lhs, Uinteger<T> rhs) noexcept(false) {
  lhs = lhs + rhs;
  return lhs;
}

template <typename U, typename T, typename = std::enable_if_t<std::is_convertible_v<U, T>>>
constexpr Uinteger<T> operator+(U lhs, Uinteger<T> rhs) noexcept(false) {
  return Uinteger<T>(lhs) + rhs;
}

template <typename T>
constexpr Uinteger<T> &operator-=(Uinteger<T> lhs, Uinteger<T> rhs) noexcept(false) {
  lhs = lhs - rhs;
  return lhs;
}

template <typename U, typename T, typename = std::enable_if_t<std::is_convertible_v<U, T>>>
constexpr Uinteger<T> operator-(U lhs, Uinteger<T> rhs) noexcept(false) {
  return Uinteger<T>(lhs) - rhs;
}

template <typename T>
constexpr Uinteger<T> &operator/=(Uinteger<T> lhs, Uinteger<T> rhs) noexcept(false) {
  lhs = lhs / rhs;
  return lhs;
}

template <typename U, typename T, typename = std::enable_if_t<std::is_convertible_v<U, T>>>
constexpr Uinteger<T> operator/(U lhs, Uinteger<T> rhs) noexcept(false) {
  return Uinteger<T>(lhs) / rhs;
}

template <typename T>
constexpr Uinteger<T> &operator*=(Uinteger<T> lhs, Uinteger<T> rhs) noexcept(false) {
  lhs = lhs * rhs;
  return lhs;
}

template <typename U, typename T, typename = std::enable_if_t<std::is_convertible_v<U, T>>>
constexpr Uinteger<T> operator*(U lhs, Uinteger<T> rhs) noexcept(false) {
  return Uinteger<T>(lhs) / rhs;
}

template <typename T>
constexpr bool operator==(Uinteger<T> lhs, Uinteger<T> rhs) {
  return lhs == rhs;
}

template <typename T>
constexpr bool operator!=(Uinteger<T> lhs, Uinteger<T> rhs) {
  return !(lhs == rhs);
}

template <typename U, typename T, typename = std::enable_if_t<std::is_convertible_v<U, T>>>
constexpr bool operator==(U lhs, Uinteger<T> rhs) noexcept {
  return Uinteger<T>(lhs) == rhs;
}

using u8 = Uinteger<uint8_t>;
using u16 = Uinteger<uint16_t>;
using u32 = Uinteger<uint32_t>;
using u64 = Uinteger<uint64_t>;
using u128 = Uinteger<uint128>;

}  // namespace numbers

#endif