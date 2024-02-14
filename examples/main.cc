#include <iostream>
#include <tuple>

#include "numbers.h"

void checked_sub_example() {
  std::cout << "==== checked_sub_example ==== \n";
  Int8 a = Int8::MIN;
  std::cout << a << '\n';
  std::optional<Int8> ret = a.checked_sub(1);
  if (ret) {
    std::cout << ret.value() << '\n';
  } else {
    std::cout << "Overflow!\n";
  }
}

void overflowing_div_example() {
  std::cout << "==== overflowing_div_example ==== \n";
  Int16 a = 40_i16;
  Int16 b = 2_i16;
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
  Int64 a = 40_i64;
  Int64 b = Int64::MAX;
  std::cout << "a= " << a << ", b= " << b << '\n';
  Int64 ret = a.saturating_mul(b);
  std::cout << ret << '\n';
}

int main(int argc, char const *argv[]) {
  auto a = 100_i8;
  std::cout << a << '\n';
  try {
    a = a + a;
    std::cout << a << '\n';
  } catch (std::runtime_error err) {
    std::cout << "Catch error: " << err.what() << '\n';
  }

  auto b = 127_i8;
  Int8 c = 0;
  try {
    Int8 ret = c - b;
    std::cout << ret << '\n';
    c = -10;
    ret = c - b;
    std::cout << ret << '\n';
  } catch (std::runtime_error err) {
    std::cout << "Catch error: " << err.what() << '\n';
  }

  auto d = static_cast<Int16>(b);
  try {
    d = d + d;
    std::cout << d << '\n';
  } catch (std::runtime_error err) {
    std::cout << "Catch error: " << err.what() << '\n';
  }

  checked_sub_example();
  overflowing_div_example();
  saturating_mul_example();

  return 0;
}
