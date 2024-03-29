#include "numbers.h"

int main(int argc, char const *argv[]) {
  {
    std::cout << "==== hash example for i128 ==== \n";
    std::hash<numbers::i128> hasher;
    numbers::i128 a = 9;
    std::cout << "a= " << a << ", hash(a)= " << hasher(a) << '\n';
    numbers::i128 b = 10;
    std::cout << "b= " << b << ", hash(b)= " << hasher(b) << '\n';
    numbers::i128 c = 8526517779554707146;
    std::cout << "c= " << c << ", hash(c)= " << hasher(c) << '\n';
  }

  {
    std::cout << "==== hash example for u128 ==== \n";
    std::hash<numbers::u128> hasher;
    numbers::u128 a = 9;
    std::cout << "a= " << a << ", hash(a)= " << hasher(a) << '\n';
    numbers::u128 b = 8526517779554707146;
    std::cout << "b= " << b << ", hash(b)= " << hasher(b) << '\n';
  }
}