#include <iostream>
#include "numbers.h"

int main(int argc, char const *argv[]) {
  std::cout << "==== conversion example ==== \n";
  numbers::u128 a = 35.6;
  numbers::i128 b = 35.6;
  numbers::i64 c = 135.6;
  std::cout << "a = " << a << ", b = " << b << ", c = " << c << '\n';
  // do some conversions
  a = static_cast<numbers::u128>(c);
  b = static_cast<numbers::i128>(c);
  std::cout << "a = " << a << ", b = " << b << ", c = " << c << '\n';

  b = 345;
  c = static_cast<numbers::i64>(b);
  std::cout << "a = " << a << ", b = " << b << ", c = " << c << '\n';
  return 0;
}
