#ifndef HEADER_INTEGER_H
#define HEADER_INTEGER_H

#include <iostream>
#include <limits>
#include <optional>
#include <type_traits>

template <typename T, typename = std::enable_if_t<std::is_integral<T>::value>>
class Integer {
 public:
  constexpr static T MIN = std::numeric_limits<T>::min();
  constexpr static T MAX = std::numeric_limits<T>::max();

  Integer() noexcept : num_{} {}
  Integer(T num) noexcept : num_{num} {}

  constexpr Integer operator+(const Integer<T> &other) noexcept(false) {
    if (add_overflow(num_, other.num_)) {
      throw std::runtime_error("add overflow");
    }
    return Integer(num_ + other.num_);
  }

  constexpr Integer wrapping_add(const Integer<T> &other) noexcept { return Integer(num_ + other.num_); }

  constexpr std::optional<Integer> checked_add(const Integer<T> &other) noexcept {
    if (add_overflow(num_, other.num_)) {
      return {};
    }
    return Integer(num_ + other.num_);
  }

  constexpr std::tuple<Integer<T>, bool> overflowing_add(const Integer<T> &other) noexcept {
    if (add_overflow(num_, other.num_)) {
      return {{}, true};
    }
    return {Integer(num_ + other.num_), false};
  }

  constexpr Integer<T> saturating_add(const Integer<T> &other) noexcept {
    T ret = num_ + other.num_;
    if (add_overflow(num_, other.num_)) {
      return Integer(is_positive(ret) ? MIN : MAX);
    }
    return Integer(ret);
  }

  constexpr Integer operator-(const Integer<T> &other) noexcept(false) {
    if (sub_overflow(num_, other.num_)) {
      throw std::runtime_error("sub overflow");
    }
    return Integer(num_ - other.num_);
  }

  constexpr Integer wrapping_sub(const Integer<T> &other) noexcept { return Integer(num_ - other.num_); }

  constexpr std::optional<Integer> checked_sub(const Integer<T> &other) noexcept {
    if (sub_overflow(num_, other.num_)) {
      return {};
    }
    return Integer(num_ - other.num_);
  }

  constexpr std::tuple<Integer<T>, bool> overflowing_sub(const Integer<T> &other) noexcept {
    if (sub_overflow(num_, other.num_)) {
      return {{}, true};
    }
    return {Integer(num_ - other.num_), false};
  }

  constexpr Integer saturating_sub(const Integer<T> &other) noexcept {
    T ret = num_ - other.num_;
    if (sub_overflow(num_, other.num_)) {
      return is_positive(ret) ? MIN : MAX;
    }
    return Integer(ret);
  }

  constexpr Integer operator/(const Integer<T> &other) {
    if (div_overflow(num_, other.num_)) {
      throw std::runtime_error("div overflow");
    }
    return Integer(num_ / other.num_);
  }

  constexpr Integer wrapping_div(const Integer<T> &other) noexcept(false) { return Integer(num_ / other.num_); }

  constexpr std::optional<Integer> checked_div(const Integer<T> &other) noexcept {
    if (div_overflow(num_, other.num_)) {
      return {};
    }
    return Integer(num_ / other.num_);
  }

  constexpr std::tuple<Integer<T>, bool> overflowing_div(const Integer<T> &other) noexcept {
    if (div_overflow(num_, other.num_)) {
      return {{}, true};
    }
    return {Integer(num_ / other.num_), false};
  }

  constexpr Integer saturating_div(const Integer<T> &other) noexcept {
    if (div_overflow(num_, other.num_)) {
      return MIN;
    }
    return Integer(num_ / other.num_);
  }

  constexpr Integer operator*(const Integer<T> &other) {
    if (mul_overflow(num_, other.num_)) {
      throw std::runtime_error("mul overflow");
    }
    return Integer(num_ * other.num_);
  }

  constexpr Integer wrapping_mul(const Integer<T> &other) noexcept(false) { return Integer(num_ * other.num_); }

  constexpr std::optional<Integer> checked_mul(const Integer<T> &other) noexcept {
    if (mul_overflow(num_, other.num_)) {
      return {};
    }
    return Integer(num_ * other.num_);
  }

  constexpr std::tuple<Integer<T>, bool> overflowing_mul(const Integer<T> &other) noexcept {
    if (mul_overflow(num_, other.num_)) {
      return {{}, true};
    }
    return {Integer(num_ * other.num_), false};
  }

  constexpr Integer saturating_mul(const Integer<T> &other) noexcept {
    if (mul_overflow(num_, other.num_)) {
      return has_same_signal(num_, other.num_) ? MAX : MIN;
    }
    return Integer(num_ * other.num_);
  }

  constexpr Integer abs() noexcept(false) {
    if (num_ == MIN) {
      throw std::runtime_error("abs overflow");
    }
    return Integer(is_positive(num_) ? num_ : -num_);
  }

  constexpr std::optional<Integer> checked_abs() noexcept {
    if (num_ == MIN) {
      return {};
    }
    return Integer(is_positive(num_) ? num_ : -num_);
  }

  constexpr std::tuple<Integer, bool> overflowing_abs() noexcept {
    if (num_ == MIN) {
      return {Integer(MIN), true};
    }
    return {Integer(is_positive(num_) ? num_ : -num_), false};
  }

  constexpr Integer wrapping_abs() noexcept {
    if (num_ == MIN) {
      return Integer(MIN);
    }
    return Integer(is_positive(num_) ? num_ : -num_);
  }

  constexpr Integer saturating_abs() noexcept {
    if (num_ == MIN) {
      return Integer(MAX);
    }
    return Integer(is_positive(num_) ? num_ : -num_);
  }

  constexpr bool operator==(const Integer<T> &other) const { return num_ == other.num_; }
  constexpr bool operator==(const T &other) const { return num_ == other; }

  constexpr bool operator<(const Integer<T> &other) const { return num_ < other.num_; }
  constexpr bool operator<(const T &other) const { return num_ < other; }

  constexpr bool operator>(const Integer<T> &other) const { return num_ > other.num_; }
  constexpr bool operator>(const T &other) const { return num_ > other; }

  // prefix ++
  constexpr Integer &operator++() {
    if (add_overflow(num_, 1)) {
      throw std::runtime_error("prefix ++ overflow");
    }
    num_ += 1;
    return *this;
  }

  // postfix ++
  constexpr Integer operator++(int) {
    if (add_overflow(num_, 1)) {
      throw std::runtime_error("postfix ++ overflow");
    }
    Integer<T> tmp = *this;
    num_ += 1;
    return tmp;
  }

  template <typename U, typename = std::enable_if<std::is_convertible_v<T, U>>>
  explicit operator Integer<U>() const {
    return Integer<U>(static_cast<U>(num_));
  }

  explicit operator T() const { return num_; }

  friend std::ostream &operator<<(std::ostream &os, const Integer<T> &num) {
    if constexpr (std::is_same<T, int8_t>::value) {
      os << static_cast<int16_t>(num.num_);
    } else {
      os << num.num_;
    }
    return os;
  }

 private:
  constexpr bool add_overflow(T a, T b) const {
    if (!has_same_signal(a, b)) {
      return false;
    }
    return !has_same_signal(a, a + b);
  }

  constexpr bool sub_overflow(T minuend, T subtrahend) const {
    if (has_same_signal(minuend, subtrahend)) {
      return false;
    }
    return !has_same_signal(minuend, minuend - subtrahend);
  }

  constexpr bool div_overflow(T a, T b) const { return a == MIN && b == -1; }

  constexpr bool mul_overflow(T a, T b) const {
    T res;
    return __builtin_mul_overflow(a, b, &res);
  }

  constexpr bool has_same_signal(T a, T b) const { return is_positive(a) == is_positive(b); }

  constexpr bool is_positive(T num) const { return num >= 0; }

  T num_;
};

using Int8 = Integer<int8_t>;
using Int16 = Integer<int16_t>;
using Int32 = Integer<int32_t>;
using Int64 = Integer<int64_t>;

Integer<int8_t> operator"" _i8(unsigned long long val) { return Integer<int8_t>(val); }

Integer<int16_t> operator"" _i16(unsigned long long val) { return Integer<int16_t>(val); }

Integer<int32_t> operator"" _i32(unsigned long long val) { return Integer<int32_t>(val); }

Integer<int64_t> operator"" _i64(unsigned long long val) { return Integer<int64_t>(val); }

#endif