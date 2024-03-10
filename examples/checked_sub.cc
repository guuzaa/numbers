#include <iostream>
#include "numbers.h"

int main(int argc, char const *argv[]) {
  std::cout << "==== checked_sub_example ==== \n";
  numbers::i8 a = numbers::i8::MIN;
  std::cout << a << '\n';
  std::optional<numbers::i8> ret = a.checked_sub(1);
  if (ret) {
    std::cout << ret.value() << '\n';
  } else {
    std::cout << "Overflow!\n";
  }
  return 0;
}
