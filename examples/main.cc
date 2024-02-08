#include <iostream>

#include "integer.hh"

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

  return 0;
}
