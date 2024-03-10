#include <iostream>
#include "numbers.h"

int main(int argc, char const *argv[]) {
  std::cout << "==== saturating_mul_example ==== \n";
  numbers::i64 a = 40;
  numbers::i64 b = numbers::i64::MAX;
  std::cout << "a= " << a << ", b= " << b << '\n';
  numbers::i64 ret = a.saturating_mul(b);
  std::cout << ret << '\n';
  return 0;
}
