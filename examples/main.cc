#include <iostream>
#include <tuple>

#include "numbers.h"

void checked_sub_example() {
  std::cout << "==== checked_sub_example ==== \n";
  numbers::i8 a = numbers::i8::MIN;
  std::cout << a << '\n';
  std::optional<numbers::i8> ret = a.checked_sub(1);
  if (ret) {
    std::cout << ret.value() << '\n';
  } else {
    std::cout << "Overflow!\n";
  }
}

void overflowing_div_example() {
  std::cout << "==== overflowing_div_example ==== \n";
  numbers::i16 a = 40;
  numbers::i16 b = 2;
  auto [ret, overflowing] = a.overflowing_div(b);
  std::cout << "a= " << a << ", b= " << b << '\n';
  if (!overflowing) {
    std::cout << ret << '\n';
  } else {
    std::cout << "Overflow!\n";
  }
}

void saturating_mul_example() {
  std::cout << "==== saturating_mul_example ==== \n";
  numbers::i64 a = 40;
  numbers::i64 b = numbers::i64::MAX;
  std::cout << "a= " << a << ", b= " << b << '\n';
  numbers::i64 ret = a.saturating_mul(b);
  std::cout << ret << '\n';
}

void i128_example() {
  std::cout << "==== i128_example ==== \n";
  numbers::i128 a = 40;
  numbers::i128 max = numbers::i128::MAX;
  numbers::i128 min = numbers::i128::MIN;
  std::cout << "a= " << a << ", max= " << max << ", min= " << min << '\n';
  numbers::i128 ret = max - a;
  std::cout << "max - a = " << ret << '\n';
}

void uint128_example() {
  std::cout << "==== uint128_example ==== \n";
  numbers::uint128 a = numbers::uint128(0) - 1;
  numbers::uint128 max = std::numeric_limits<numbers::uint128>::max();
  numbers::uint128 min = std::numeric_limits<numbers::uint128>::min();
  std::cout << "a= " << a << ", max= " << max << ", min= " << min << '\n';
}

int main(int argc, char const *argv[]) {
  auto a = 100;
  std::cout << a << '\n';
  try {
    a = a + a;
    std::cout << a << '\n';
  } catch (std::runtime_error &err) {
    std::cout << "Catch error: " << err.what() << '\n';
  }

  numbers::i8 b = 127;
  numbers::i8 c = 0;
  try {
    numbers::i8 ret = c - b;
    std::cout << ret << '\n';
    c = -10;
    ret = c - b;
    std::cout << ret << '\n';
  } catch (std::runtime_error &err) {
    std::cout << "Catch error: " << err.what() << '\n';
  }

  auto d = static_cast<numbers::i16>(b);
  try {
    d = d + d;
    std::cout << d << '\n';
  } catch (std::runtime_error &err) {
    std::cout << "Catch error: " << err.what() << '\n';
  }

  checked_sub_example();
  overflowing_div_example();
  saturating_mul_example();
  i128_example();
  uint128_example();

  return 0;
}
