#ifndef NUMBERS_BITS_HH
#define NUMBERS_BITS_HH

#include <type_traits>

#include "internal/config.h"
#include "internal/bits.hh"

#if NUMBERS_INTERNAL_CPLUSPLUS_LANG >= 202002L
#include <bit>
#endif

// check if the compiler supports <bit> header
#if defined(__cpp_lib_bitops) && __cpp_lib_bitops >= 201907L

using std::countl_one;
using std::countl_zero;

#else

// Counting functions
//
// While these functions are typically constexpr, on some platforms, they may
// not be marked as constexpr due to constraints of the compiler/available
// intrinsics.
template <class T>
inline typename std::enable_if<std::is_unsigned_v<T>, int>::type countl_zero(T x) noexcept {
  return numbers_internal::count_leading_zeroes(x);
}

template <class T>
inline typename std::enable_if<std::is_unsigned_v<T>, int>::type countl_one(T x) noexcept {
  // Avoid integer promotion to a wider type
  return countl_zero(static_cast<T>(~x));
}

#endif

#endif