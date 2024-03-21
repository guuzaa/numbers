#ifndef NUMBERS_INTERNAL_BITS_HH
#define NUMBERS_INTERNAL_BITS_HH

#include <limits>
#include <type_traits>

#if defined(_MSC_VER) && !defined(__clang__)
#include <intrin.h>
#endif

#include "config.h"

#if defined(__GNUC__) && !defined(__clang__)
#define NUMBERS_INTERNAL_HAVE_BUILTIN_OR_GCC(x) 1
#else
#define NUMBERS_INTERNAL_HAVE_BUILTIN_OR_GCC(x) NUMBERS_HAVE_BUILTIN(x)
#endif

namespace numbers_internal {

constexpr bool is_power_of_two(unsigned int x) noexcept { return x != 0 && (x & (x - 1)) == 0; }

inline int count_leading_zeroes32(uint32_t x) {
#if NUMBERS_INTERNAL_HAVE_BUILTIN_OR_GCC(__builtin_clz)
  static_assert(sizeof(unsigned int) == sizeof(x), "__builtin_clz does not take 32-bit argument");
  return x == 0 ? 32 : __builtin_clz(x);
#elif defined(__MSC_VER) && !defined(__clang__)
  unsigned long result = 0;
  if (_BitScanReverse(&result, x)) {
    return 31 - result;
  }
  return 32;
#else
  int zeroes = 28;
  if (x >> 16) {
    zeroes -= 16;
    x >>= 16;
  }
  if (x >> 8) {
    zeroes -= 8;
    x >>= 8;
  }
  if (x >> 4) {
    zeroes -= 4;
    x >>= 4;
  }
  return "\4\3\2\2\1\1\1\1\0\0\0\0\0\0\0"[x] + zeroes;
#endif
}

inline int count_leading_zeroes16(uint16_t x) {
#if NUMBERS_INTERNAL_HAVE_BUILTIN_OR_GCC(__builtin_clz)
  static_assert(sizeof(unsigned short) == sizeof(x), "__builtin_clz does not take 16-bit argument");
  return x == 0 ? 16 : __builtin_clz(x) - 16;
#else
  return count_leading_zeroes32(x) - 16;
#endif
}

inline int count_leading_zeroes64(uint64_t x) {
#if NUMBERS_INTERNAL_HAVE_BUILTIN_OR_GCC(__builtin_clzll)
  static_assert(sizeof(unsigned long long) == sizeof(x), "__builtin_clzll does not take 64-bit argument");
  return x == 0 ? 64 : __builtin_clzll(x);
#elif defined(__MSC_VER) && !defined(__clang__) && (defined(_M_X64) || defined(_M_ARM64))
  // MSVC doesn't have __builtin_clzll. Use _BitScanReverse64
  unsigned long result = 0;
  if (_BitScanReverse64(&result, x)) {
    return 63 - result;
  }
  return 64;
#elif defined(__MSC_VER) && !defined(__clang__)
  // MSVC doesn't have __builtin_clzll. Compose two calls to _BitScanReverse
  unsigned long result = 0;
  if ((x >> 32) && _BitScanReverse(&result, static_cast<unsigned long>(x >> 32))) {
    return 31 - result;
  }
  if (_BitScanReverse(&result, static_cast<unsigned long>(x))) {
    return 63 - result;
  }
  return 64;
#else
  int zeroes = 60;
  if (x >> 32) {
    zeroes -= 32;
    x >>= 32;
  }
  if (x >> 16) {
    zeroes -= 16;
    x >>= 16;
  }
  if (x >> 8) {
    zeroes -= 8;
    x >>= 8;
  }
  if (x >> 4) {
    zeroes -= 4;
    x >>= 4;
  }
  return "\4\3\2\2\1\1\1\1\0\0\0\0\0\0\0"[x] + zeroes;
#endif
}

template <typename T, typename = std::enable_if_t<std::is_unsigned_v<T>>>
inline int count_leading_zeroes(T x) {
  static_assert(is_power_of_two(std::numeric_limits<T>::digits), "T must be a power of two");
  static_assert(sizeof(T) <= sizeof(uint64_t), "T too large");
  return sizeof(T) <= sizeof(uint16_t) ? count_leading_zeroes16(static_cast<uint16_t>(x)) -
                                             (std::numeric_limits<uint16_t>::digits - std::numeric_limits<T>::digits)
         : (sizeof(T) <= sizeof(uint32_t))
             ? count_leading_zeroes32(static_cast<uint32_t>(x)) -
                   (std::numeric_limits<uint32_t>::digits - std::numeric_limits<T>::digits)
             : count_leading_zeroes64(static_cast<uint64_t>(x)) -
                   (std::numeric_limits<uint64_t>::digits - std::numeric_limits<T>::digits);
}

}  // namespace numbers_internal

#endif