#include "test/utils.hh"

#include <cctype>
#include <random>

using u32 = uint_least32_t;
using engine = std::mt19937;

int random_integer(int min, int max) {
  std::random_device os_seed;
  const u32 seed = os_seed();
  engine generator(seed);
  std::uniform_int_distribution<u32> distribute(min, max);

  return distribute(generator);
}