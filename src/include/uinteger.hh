#include <iostream>
#include <limits>
#include <optional>
#include <type_traits>

template <typename T, typename = std::enable_if_t<std::is_unsigned<T>::value>>
class Uinteger {
public:
// unimplemented!

private:
  T num_;
};