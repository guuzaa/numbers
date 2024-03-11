#include "integer.hh"

#if defined(__MSC_VER)
namespace numbers {
template <>
i8 i8::MAX = i8(max_);
template <>
i8 i8::MIN = i8(min_);

template <>
i16 i16::MAX = i16(max_);
template <>
i16 i16::MIN = i16(min_);

template <>
i32 i32::MAX = i32(max_);
template <>
i32 i32::MIN = i32(min_);

template <>
i64 i64::MAX = i64(max_);
template <>
i64 i64::MIN = i64(min_);

template <>
i128 i128::MAX = i128(max_);
template <>
i128 i128::MIN = i128(min_);
}  // namespace numbers
#endif
