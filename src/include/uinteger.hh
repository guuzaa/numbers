#ifndef HEADER_UNINTEGER_H
#define HEADER_UNINTEGER_H

#include <iostream>
#include <limits>
#include <optional>
#include <type_traits>

namespace numbers {

namespace {

template <typename T, typename = std::enable_if_t<std::is_unsigned_v<T>>>
class Uinteger {
 private:
  constexpr static T min_ = std::numeric_limits<T>::min();
  constexpr static T max_ = std::numeric_limits<T>::max();

 public:
  inline static Uinteger<T> MIN = Uinteger(min_);
  inline static Uinteger<T> MAX = Uinteger(max_);

  constexpr Uinteger() noexcept : num_{} {}
  constexpr Uinteger(T num) noexcept : num_{num} {}

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

  constexpr bool operator==(const Uinteger<T> &other) const noexcept { return num_ == other.num_; }
  constexpr bool operator==(const T &other) const noexcept { return num_ == other; }

  constexpr bool operator<(const Uinteger<T> &other) const noexcept { return num_ < other.num_; }
  constexpr bool operator<(const T &other) const noexcept { return num_ < other; }

  constexpr bool operator>(const Uinteger<T> &other) const noexcept { return num_ > other.num_; }
  constexpr bool operator>(const T &other) const noexcept { return num_ > other; }

  // prefix ++
  constexpr Uinteger &operator++() noexcept(false) {
    if (add_overflow(num_, 1)) {
      throw std::runtime_error("prefix ++ overflow");
    }
    num_ += 1;
    return *this;
  }

  // postfix ++
  constexpr Uinteger operator++(int) noexcept(false) {
    if (add_overflow(num_, 1)) {
      throw std::runtime_error("postfix ++ overflow");
    }
    Uinteger<T> tmp = *this;
    num_ += 1;
    return tmp;
  }

  template <typename U, typename = std::enable_if<std::is_convertible_v<T, U>>>
  explicit operator Uinteger<U>() const {
    return Uinteger<U>(static_cast<U>(num_));
  }

  explicit operator T() const noexcept { return num_; }

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

}  // namespace

using uint8 = Uinteger<uint8_t>;
using uint16 = Uinteger<uint16_t>;
using uint32 = Uinteger<uint32_t>;
using uint64 = Uinteger<uint64_t>;

}  // namespace numbers

#endif