#ifndef HEADER_NUMBERS_H
#define HEADER_NUMBERS_H

#include "integer.hh"
#include "uinteger.hh"

using Int8 = Integer<int8_t>;
using Int16 = Integer<int16_t>;
using Int32 = Integer<int32_t>;
using Int64 = Integer<int64_t>;

Integer<int8_t> operator"" _i8(unsigned long long val) { return Integer<int8_t>(val); }

Integer<int16_t> operator"" _i16(unsigned long long val) { return Integer<int16_t>(val); }

Integer<int32_t> operator"" _i32(unsigned long long val) { return Integer<int32_t>(val); }

Integer<int64_t> operator"" _i64(unsigned long long val) { return Integer<int64_t>(val); }

using Uint8 = Uinteger<uint8_t>;
using Uint16 = Uinteger<uint16_t>;
using Uint32 = Uinteger<uint32_t>;
using Uint64 = Uinteger<uint64_t>;

Uint8 operator"" _u8(unsigned long long val) { return Uint8(val); }

Uint16 operator"" _u16(unsigned long long val) { return Uint16(val); }

Uint32 operator"" _u32(unsigned long long val) { return Uint32(val); }

Uint64 operator"" _u64(unsigned long long val) { return Uint64(val); }

#endif