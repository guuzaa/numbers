#include <iostream>
#include <tuple>

#include "numbers.h"

void for_error() {
  size_t i;

  // error - infinite loop
  for (i = 10; i >= 0; i--) {
    printf("[ID %lu] Hello, World\n", i);
  }
}

void for_correct() {
  using namespace numbers;

  bool flag = false;
  for (u8 i = 10; !flag && i >= 0; std::tie(i, flag) = i.overflowing_sub(1)) {
    printf("[ID %u] Hello, World\n", i);
  }
}

int main(int argc, char const *argv[]) {
  for_correct();
  return 0;
}
