#ifndef NUMBERS_INTERNAL_CONFIG_H
#define NUMBERS_INTERNAL_CONFIG_H

#define NUMBERS_INTERNAL_CPLUSPLUS_LANG __cplusplus

#ifdef __has_builtin
#define NUMBERS_HAVE_BUILTIN(x) __has_builtin(x)
#else
#define NUMBERS_HAVE_BUILTIN(x) 0
#endif  // __has_builtin

#endif
