#ifndef NUMBERS_INTERNAL_CONFIG_H
#define NUMBERS_INTERNAL_CONFIG_H

#define NUMBERS_INTERNAL_CPLUSPLUS_LANG __cplusplus

#ifdef __has_builtin
#define NUMBERS_HAVE_BUILTIN(x) __has_builtin(x)
#else
#define NUMBERS_HAVE_BUILTIN(x) 0
#endif  // __has_builtin

#ifdef NUMBERS_HAVE_INTRINSTIC_INT128
#error NUMBERS_HAVE_INTRINSTIC_INT128 cannot be directly set
#elif defined(__SIZEOF_INT128__)
#if (defined(__clang__) && !defined(_WIN32)) || (defined(__GNUC__) && !defined(__clang__) && !defined(__CUDACC))
#define NUMBERS_HAVE_INTRINSTIC_INT128 1
#endif
#endif

#endif
