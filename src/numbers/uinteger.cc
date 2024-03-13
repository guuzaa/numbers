#if defined(_MSC_VER)
#include "uinteger.hh"

namespace numbers {
template <>
u8 u8::MAX = u8(max_);
template <>
u8 u8::MIN = u8(min_);

template <>
u16 u16::MAX = u16(max_);
template <>
u16 u16::MIN = u16(min_);

template <>
u32 u32::MAX = u32(max_);
template <>
u32 u32::MIN = u32(min_);

template <>
u64 u64::MAX = u64(max_);
template <>
u64 u64::MIN = u64(min_);

template <>
u128 u128::MAX = u128(max_);
template <>
u128 u128::MIN = u128(min_);
}  // namespace numbers
#endif
