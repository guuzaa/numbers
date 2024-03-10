#include <iostream>
#include "numbers.h"

int main(int argc, char const *argv[]) {
  std::cout << "==== u128_example ==== \n";
  numbers::u128 a = 40;
  numbers::u128 max = numbers::u128::MAX;
  numbers::u128 min = numbers::u128::MIN;
  std::cout << "a= " << a << ", max= " << max << ", min= " << min << '\n';
  numbers::u128 ret = max - a;
  std::cout << "max - a = " << ret << '\n';
  return 0;
}