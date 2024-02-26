#ifndef NUMBERS_INTERNAL_BITS_HH
#define NUMBERS_INTERNAL_BITS_HH

#include <type_traits>
#include <limits>

namespace numbers_internal {

constexpr bool is_power_of_two(unsigned int x) noexcept { return x != 0 && (x & (x - 1)) == 0; }

inline int count_leading_zeroes32(uint32_t x) {
  static_assert(sizeof(unsigned int) == sizeof(x), "__builtin_clz does not take 32-bit argument");
  return x == 0 ? 32 : __builtin_clz(x);
}

inline int count_leading_zeroes16(uint16_t x) {
  static_assert(sizeof(unsigned short) == sizeof(x), "__builtin_clz does not take 16-bit argument");
  return x == 0 ? 16 : __builtin_clz(x) - 16;
}

inline int count_leading_zeroes64(uint64_t x) {
  static_assert(sizeof(unsigned long long) == sizeof(x), "__builtin_clzll does not take 64-bit argument");
  return x == 0 ? 64 : __builtin_clzll(x);
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