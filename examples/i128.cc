#include "numbers.h"

int main(int argc, char const *argv[]) {
  std::cout << "==== i128_example ==== \n";
  numbers::i128 a = 40;
  numbers::i128 max = numbers::i128::MAX;
  numbers::i128 min = numbers::i128::MIN;
  std::cout << "a= " << a << ", max= " << max << ", min= " << min << '\n';
  numbers::i128 ret = max - a;
  std::cout << "max - a = " << ret << '\n';
  return 0;
}
