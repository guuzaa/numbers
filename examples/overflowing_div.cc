#include <iostream>
#include <tuple>

#include "numbers.h"

int main(int argc, char const *argv[]) {
  std::cout << "==== overflowing_div_example ==== \n";
  numbers::i16 a = 40;
  numbers::i16 b = 2;
  auto [ret, overflowing] = a.overflowing_div(b);
  std::cout << "a= " << a << ", b= " << b << '\n';
  if (!overflowing) {
    std::cout << "Never Overflow!\tret = " << ret << '\n';
  } else {
    std::cout << "Overflow!\n";
  }
  return 0;
}
