#include <iostream>
#include <tuple>

#include "numbers.h"

void checked_sub_example() {
  std::cout << "==== checked_sub_example ==== \n";
  numbers::int8 a = numbers::int8::MIN;
  std::cout << a << '\n';
  std::optional<numbers::int8> ret = a.checked_sub(1);
  if (ret) {
    std::cout << ret.value() << '\n';
  } else {
    std::cout << "Overflow!\n";
  }
}

void overflowing_div_example() {
  std::cout << "==== overflowing_div_example ==== \n";
  numbers::int16 a = 40;
  numbers::int16 b = 2;
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
  numbers::int64 a = 40;
  numbers::int64 b = numbers::int64::MAX;
  std::cout << "a= " << a << ", b= " << b << '\n';
  numbers::int64 ret = a.saturating_mul(b);
  std::cout << ret << '\n';
}

void int128_example() {
  std::cout << "==== int128_example ==== \n";
  numbers::int128 a = 40;
  numbers::int128 max = numbers::int128::MAX;
  numbers::int128 min = numbers::int128::MIN;
  std::cout << "a= " << a << ", max= " << max << ", min= " << min << '\n';
  numbers::int128 ret = max - a;
  std::cout << "max - a = " << ret << '\n';
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

  numbers::int8 b = 127;
  numbers::int8 c = 0;
  try {
    numbers::int8 ret = c - b;
    std::cout << ret << '\n';
    c = -10;
    ret = c - b;
    std::cout << ret << '\n';
  } catch (std::runtime_error &err) {
    std::cout << "Catch error: " << err.what() << '\n';
  }

  auto d = static_cast<numbers::int16>(b);
  try {
    d = d + d;
    std::cout << d << '\n';
  } catch (std::runtime_error &err) {
    std::cout << "Catch error: " << err.what() << '\n';
  }

  checked_sub_example();
  overflowing_div_example();
  saturating_mul_example();
  int128_example();

  return 0;
}
