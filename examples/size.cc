#include "numbers.h"

int main(int argc, char const *argv[])
{
  std::cout << "==== size example ==== \n";
  std::cout << "sizeof(numbers::i8) = "<< sizeof(numbers::i8) << '\n';
  std::cout << "sizeof(numbers::i16) = "<< sizeof(numbers::i16) << '\n';
  std::cout << "sizeof(numbers::i32) = "<< sizeof(numbers::i32) << '\n';
  std::cout << "sizeof(numbers::i64) = "<< sizeof(numbers::i64) << '\n';
  std::cout << "sizeof(numbers::i128) = "<< sizeof(numbers::i128) << '\n';

  std::cout << "sizeof(numbers::u8) = "<< sizeof(numbers::u8) << '\n';
  std::cout << "sizeof(numbers::u16) = "<< sizeof(numbers::u16) << '\n';
  std::cout << "sizeof(numbers::u32) = "<< sizeof(numbers::u32) << '\n';
  std::cout << "sizeof(numbers::u64) = "<< sizeof(numbers::u64) << '\n';
  std::cout << "sizeof(numbers::u128) = "<< sizeof(numbers::u128) << '\n';
  return 0;
}
