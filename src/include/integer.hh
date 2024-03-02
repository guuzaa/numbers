#ifndef HEADER_INTEGER_H
#define HEADER_INTEGER_H

#include <iostream>
#include <limits>
#include <optional>
#include <type_traits>
#include "int128.hh"

namespace numbers {

template <typename T, typename = std::enable_if_t<std::is_signed_v<T>>>
class Integer {
 private:
  constexpr static T min_ = std::numeric_limits<T>::min();
  constexpr static T max_ = std::numeric_limits<T>::max();

 public:
  inline static Integer<T> MIN = Integer<T>(min_);
  inline static Integer<T> MAX = Integer<T>(max_);

  constexpr Integer() noexcept : num_{} {}
  template <typename U, typename = std::enable_if<std::is_convertible_v<U, T> && std::is_signed_v<U>>>
  Integer(U num) noexcept : num_{static_cast<T>(num)} {}

  constexpr Integer operator+(Integer<T> other) const noexcept(false) {
    if (add_overflow(num_, other.num_)) {
      throw std::runtime_error("add overflow");
    }
    return Integer(num_ + other.num_);
  }

  constexpr Integer wrapping_add(const Integer<T> &other) const noexcept { return Integer(num_ + other.num_); }

  constexpr std::optional<Integer> checked_add(const Integer<T> &other) const noexcept {
    if (add_overflow(num_, other.num_)) {
      return {};
    }
    return Integer(num_ + other.num_);
  }

  constexpr std::tuple<Integer<T>, bool> overflowing_add(const Integer<T> &other) const noexcept {
    return {Integer(num_ + other.num_), add_overflow(num_, other.num_)};
  }

  constexpr Integer<T> saturating_add(const Integer<T> &other) const noexcept {
    T ret = num_ + other.num_;
    if (add_overflow(num_, other.num_)) {
      return Integer(is_positive(ret) ? MIN : MAX);
    }
    return Integer(ret);
  }

  constexpr Integer operator-(const Integer<T> &other) const noexcept(false) {
    if (sub_overflow(num_, other.num_)) {
      throw std::runtime_error("sub overflow");
    }
    return Integer(num_ - other.num_);
  }

  constexpr Integer wrapping_sub(const Integer<T> &other) const noexcept { return Integer(num_ - other.num_); }

  constexpr std::optional<Integer> checked_sub(const Integer<T> &other) const noexcept {
    if (sub_overflow(num_, other.num_)) {
      return {};
    }
    return Integer(num_ - other.num_);
  }

  constexpr std::tuple<Integer<T>, bool> overflowing_sub(const Integer<T> &other) const noexcept {
    return {Integer(num_ - other.num_), sub_overflow(num_, other.num_)};
  }

  constexpr Integer saturating_sub(const Integer<T> &other) const noexcept {
    T ret = num_ - other.num_;
    if (sub_overflow(num_, other.num_)) {
      return is_positive(ret) ? MIN : MAX;
    }
    return Integer(ret);
  }

  constexpr Integer operator/(const Integer<T> &other) const noexcept(false) {
    if (div_overflow(num_, other.num_)) {
      throw std::runtime_error("div overflow");
    }
    return Integer(num_ / other.num_);
  }

  constexpr Integer wrapping_div(const Integer<T> &other) const noexcept { return Integer(num_ / other.num_); }

  constexpr std::optional<Integer> checked_div(const Integer<T> &other) const noexcept {
    if (div_overflow(num_, other.num_)) {
      return {};
    }
    return Integer(num_ / other.num_);
  }

  constexpr std::tuple<Integer<T>, bool> overflowing_div(const Integer<T> &other) const noexcept {
    return {Integer(num_ / other.num_), div_overflow(num_, other.num_)};
  }

  constexpr Integer saturating_div(const Integer<T> &other) const noexcept {
    if (div_overflow(num_, other.num_)) {
      return MIN;
    }
    return Integer(num_ / other.num_);
  }

  constexpr Integer operator*(const Integer<T> &other) const noexcept(false) {
    if (mul_overflow(num_, other.num_)) {
      throw std::runtime_error("mul overflow");
    }
    return Integer(num_ * other.num_);
  }

  constexpr Integer wrapping_mul(const Integer<T> &other) const noexcept { return Integer(num_ * other.num_); }

  constexpr std::optional<Integer> checked_mul(const Integer<T> &other) const noexcept {
    if (mul_overflow(num_, other.num_)) {
      return {};
    }
    return Integer(num_ * other.num_);
  }

  constexpr std::tuple<Integer<T>, bool> overflowing_mul(const Integer<T> &other) const noexcept {
    return {Integer(num_ * other.num_), mul_overflow(num_, other.num_)};
  }

  constexpr Integer saturating_mul(const Integer<T> &other) const noexcept {
    if (mul_overflow(num_, other.num_)) {
      return has_same_signal(num_, other.num_) ? MAX : MIN;
    }
    return Integer(num_ * other.num_);
  }

  constexpr Integer abs() const noexcept(false) {
    if (num_ == min_) {
      throw std::runtime_error("abs overflow");
    }
    return Integer(is_positive(num_) ? num_ : -num_);
  }

  constexpr std::optional<Integer> checked_abs() const noexcept {
    if (num_ == min_) {
      return {};
    }
    return Integer(is_positive(num_) ? num_ : -num_);
  }

  constexpr std::tuple<Integer, bool> overflowing_abs() const noexcept {
    if (num_ == min_) {
      return {MIN, true};
    }
    return {Integer(is_positive(num_) ? num_ : -num_), false};
  }

  constexpr Integer wrapping_abs() const noexcept {
    if (num_ == min_) {
      return MIN;
    }
    return Integer(is_positive(num_) ? num_ : -num_);
  }

  constexpr Integer saturating_abs() const noexcept {
    if (num_ == min_) {
      return MAX;
    }
    return Integer(is_positive(num_) ? num_ : -num_);
  }

  constexpr Integer operator-() const noexcept(false) {
    if (num_ == min_) {
      throw std::runtime_error("neg overflow");
    }
    return Integer(-num_);
  }

  constexpr std::optional<Integer> checked_neg() const noexcept {
    if (num_ == min_) {
      return {};
    }
    return Integer(-num_);
  }

  constexpr std::tuple<Integer, bool> overflowing_neg() const noexcept {
    if (num_ == min_) {
      return {MIN, true};
    }
    return {Integer(-num_), false};
  }

  constexpr Integer wrapping_neg() const noexcept {
    if (num_ == min_) {
      return MIN;
    }
    return Integer(-num_);
  }

  constexpr Integer saturating_neg() const noexcept {
    if (num_ == min_) {
      return MAX;
    }
    return Integer(-num_);
  }

  constexpr bool operator==(const Integer<T> &other) const noexcept { return num_ == other.num_; }
  constexpr bool operator<(const Integer<T> &other) const noexcept { return num_ < other.num_; }
  constexpr bool operator>(const Integer<T> &other) const noexcept { return num_ > other.num_; }

  Integer &operator+=(Integer other) {
    *this = *this + other;
    return *this;
  }

  Integer &operator-=(Integer other) {
    *this = *this - other;
    return *this;
  }

  Integer &operator/=(Integer other) {
    *this = *this / other;
    return *this;
  }

  Integer &operator*=(Integer other) {
    *this = *this * other;
    return *this;
  }

  Integer operator<<(int amount) {
    num_ <<= amount;
    return *this;
  }

  Integer operator>>(int amount) {
    num_ >>= amount;
    return *this;
  }

  Integer &operator<<=(int amount) {
    *this <<= amount;
    return *this;
  }

  Integer &operator>>=(int amount) {
    *this >>= amount;
    return *this;
  }

  // prefix ++
  Integer &operator++() noexcept(false) {
    *this += Integer(1);
    return *this;
  }

  // postfix ++
  Integer operator++(int) noexcept(false) {
    Integer<T> tmp = *this;
    *this += Integer(1);
    return tmp;
  }

  // prefix --
  Integer &operator--() noexcept(false) {
    *this -= Integer(1);
    return *this;
  }

  // postfix --
  Integer operator--(int) noexcept(false) {
    Integer<T> tmp = *this;
    *this -= Integer(1);
    return tmp;
  }

  template <typename U, typename = std::enable_if<std::is_convertible_v<U, T>>>
  explicit operator Integer<U>() const {
    return Integer<U>(static_cast<U>(num_));
  }

  template <typename U, typename = std::enable_if<std::is_convertible_v<U, T>>>
  explicit operator U() const noexcept {
    return static_cast<U>(num_);
  }

  friend std::ostream &operator<<(std::ostream &os, const Integer<T> &num) {
    if constexpr (std::is_same<T, int8_t>::value) {
      os << static_cast<int16_t>(num.num_);
    } else {
      os << num.num_;
    }
    return os;
  }

 private:
  constexpr bool add_overflow(T a, T b) const noexcept { return has_same_signal(a, b) && !has_same_signal(a, a + b); }

  constexpr bool sub_overflow(T minuend, T subtrahend) const noexcept {
    return !has_same_signal(minuend, subtrahend) && !has_same_signal(minuend, minuend - subtrahend);
  }

  constexpr bool div_overflow(T a, T b) const noexcept { return a == min_ && b == -1; }

  constexpr bool mul_overflow(T a, T b) const {
    if constexpr (std::is_same_v<T, int128>) {
      // TODO Unimplemented
      return false;
    } else {
      T res;
      return __builtin_mul_overflow(a, b, &res);
    }
  }

  constexpr bool has_same_signal(T a, T b) const noexcept { return is_positive(a) == is_positive(b); }

  constexpr bool is_positive(T num) const noexcept { return num >= 0; }

  T num_;
};

template <typename T>
constexpr Integer<T> &operator+=(Integer<T> lhs, Integer<T> rhs) noexcept(false) {
  lhs = lhs + rhs;
  return lhs;
}

template <typename U, typename T, typename = std::enable_if_t<std::is_signed_v<U> && std::is_convertible_v<U, T>>>
constexpr Integer<T> operator+(U lhs, Integer<T> rhs) noexcept(false) {
  return Integer<T>(lhs) + rhs;
}

template <typename T>
constexpr Integer<T> &operator-=(Integer<T> lhs, Integer<T> rhs) noexcept(false) {
  lhs = lhs - rhs;
  return lhs;
}

template <typename T>
constexpr Integer<T> &operator/=(Integer<T> lhs, Integer<T> rhs) noexcept(false) {
  lhs = lhs / rhs;
  return lhs;
}

template <typename T>
constexpr Integer<T> &operator*=(Integer<T> lhs, Integer<T> rhs) noexcept(false) {
  lhs = lhs * rhs;
  return lhs;
}

template <typename T>
constexpr bool operator==(Integer<T> lhs, Integer<T> rhs) {
  return lhs == rhs;
}

template <typename T>
constexpr bool operator!=(Integer<T> lhs, Integer<T> rhs) {
  return !(lhs == rhs);
}

template <typename U, typename T, typename = std::enable_if_t<std::is_signed_v<U> && std::is_convertible_v<U, T>>>
constexpr bool operator==(U lhs, Integer<T> rhs) noexcept {
  return Integer<T>(lhs) == rhs;
}

using int8 = Integer<int8_t>;
using int16 = Integer<int16_t>;
using int32 = Integer<int32_t>;
using int64 = Integer<int64_t>;
using i128 = Integer<int128>;

}  // namespace numbers

#endif