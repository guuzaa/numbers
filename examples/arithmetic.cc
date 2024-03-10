#include <iostream>
#include "numbers.h"

int main(int argc, char const *argv[]) {
  std::cout << "==== arithmetic_example ==== \n";
  numbers::i8 a = 100;
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

  return 0;
}
