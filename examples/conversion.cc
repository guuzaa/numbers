#include <iostream>
#include "numbers.h"

int main(int argc, char const *argv[]) {
  numbers::u128 a = 35.6;
  numbers::i128 b = 35.6;
  numbers::i64 c = 35.6;
  std::cout << "a = " << a << ", b = " << b << ", c = " << c << '\n';
  return 0;
}
