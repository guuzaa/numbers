// Copyright 2017 The Abseil Authors.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//      https://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
// Modified from abseil-app guuzaa

#include <cassert>
#include <iomanip>
#include <ostream>
#include <sstream>

#include "bits.hh"
#include "int128.hh"

namespace numbers {

namespace {

inline int Fls128(uint128 n) {
  if (uint64_t hi = Uint128High64(n)) {
    assert(hi != 0);
    return 127 - countl_zero(hi);
  }
  const uint64_t low = Uint128Low64(n);
  assert(low != 0);
  return 63 - countl_zero(low);
}

// Long division/modulo for uint128
inline void DivModImpl(uint128 dividend, uint128 divisor, uint128 *quotient_ret, uint128 *remainder_ret) {
  assert(divisor != 0);

  if (divisor > dividend) {
    *quotient_ret = 0;
    *remainder_ret = dividend;
    return;
  }

  if (divisor == dividend) {
    *quotient_ret = 1;
    *remainder_ret = 0;
  }

  uint128 denominator = divisor;
  uint128 quotient = 0;

  // Left aligns the MSB of the denominator and the dividend.
  const int shift = Fls128(dividend) - Fls128(denominator);
  denominator <<= shift;

  for (int i = 0; i <= shift; ++i) {
    quotient <<= 1;
    if (dividend >= denominator) {
      dividend -= denominator;
      quotient |= 1;
    }
    denominator >>= 1;
  }

  *quotient_ret = quotient;
  *remainder_ret = dividend;
}
}  // namespace

namespace {
std::string uint128_to_formatted_string(uint128 v, std::ios_base::fmtflags flags) {
  uint128 div;
  int div_base_log;
  switch (flags & std::ios::basefield) {
    case std::ios::hex:
      div = 0x1000000000000000;  // 16^15
      div_base_log = 15;
      break;
    case std::ios::oct:
      div = 01000000000000000000000;
      div_base_log = 21;
      break;
    default:  // std::ios::dec
      div = 10000000000000000000u;
      div_base_log = 19;
      break;
  }

  std::ostringstream os;
  std::ios_base::fmtflags copy_mask = std::ios::basefield | std::ios::showbase | std::ios::uppercase;
  os.setf(flags & copy_mask, copy_mask);
  uint128 high = v;
  uint128 low;
  DivModImpl(high, div, &high, &low);
  uint128 mid;
  DivModImpl(high, div, &high, &mid);
  if (Uint128Low64(high) != 0) {
    os << Uint128Low64(high);
    os << std::noshowbase << std::setfill('0') << std::setw(div_base_log);
    os << Uint128Low64(mid);
    os << std::setw(div_base_log);
  } else if (Uint128Low64(mid) != 0) {
    os << Uint128Low64(mid);
    os << std::noshowbase << std::setfill('0') << std::setw(div_base_log);
  }
  os << Uint128Low64(low);
  return os.str();
}

}  // namespace

}  // namespace numbers

namespace numbers {

uint128 operator/(uint128 lhs, uint128 rhs) {
  uint128 quotient = 0;
  uint128 remainder = 0;
  DivModImpl(lhs, rhs, &quotient, &remainder);
  return quotient;
}

namespace {
uint128 UnsignedAbsoluteValue(int128 v) { return int128_high64(v) < 0 ? -uint128(v) : uint128(v); }

}  // namespace

int128 operator/(int128 lhs, int128 rhs) {
  // assert(lhs != int128::MIN || rhs != -1);  ignore overflowing

  uint128 quotient = 0;
  uint128 remainder = 0;
  DivModImpl(UnsignedAbsoluteValue(lhs), UnsignedAbsoluteValue(rhs), &quotient, &remainder);
  if ((int128_high64(lhs) < 0) != (int128_high64(rhs) < 0)) {
    quotient = -quotient;
  }
  return make_int128(int128_internal::BitCastToSigned(Uint128High64(quotient)), Uint128Low64(quotient));
}

int128 operator%(int128 lhs, int128 rhs) {
  assert(lhs != int128_min() || rhs != -1);  // overflowing

  uint128 quotient = 0;
  uint128 remainder = 0;
  DivModImpl(UnsignedAbsoluteValue(lhs), UnsignedAbsoluteValue(rhs), &quotient, &remainder);
  if (int128_high64(lhs) < 0) {
    remainder = -remainder;
  }
  return make_int128(int128_internal::BitCastToSigned(Uint128High64(remainder)), Uint128Low64(remainder));
}

std::ostream &operator<<(std::ostream &os, int128 v) {
  std::ios_base::fmtflags flags = os.flags();
  std::string rep;
  // Add the sign if needed
  bool print_as_decimal =
      (flags & std::ios::basefield) == std::ios::dec || (flags & std::ios::basefield) == std::ios_base::fmtflags();
  if (print_as_decimal) {
    if (int128_high64(v) < 0) {
      rep = "-";
    } else if (flags & std::ios::showpos) {
      rep = "+";
    }
  }

  rep.append(uint128_to_formatted_string((print_as_decimal ? UnsignedAbsoluteValue(v) : uint128(v)), os.flags()));

  // Add the requisite padding
  std::streamsize width = os.width(0);
  if (static_cast<size_t>(width) > rep.size()) {
    const size_t count = static_cast<size_t>(width) - rep.size();
    switch (flags & std::ios::adjustfield) {
      case std::ios::left:
        rep.append(count, os.fill());
        break;
      case std::ios::internal:
        if (print_as_decimal && (rep[0] == '+' || rep[0] == '-')) {
          rep.insert(size_t{1}, count, os.fill());
        } else if ((flags & std::ios::basefield) == std::ios::hex && (flags & std::ios::showbase) && v != 0) {
          rep.insert(size_t{2}, count, os.fill());
        } else {
          rep.insert(size_t{0}, count, os.fill());
        }
        break;
      default:  // std::ios::right
        rep.insert(0, count, os.fill());
        break;
    }
  }
  return os << rep;
}

std::string int128::to_string() const {
  return uint128_to_formatted_string(*this, std::ios_base::dec);
}

}  // namespace numbers