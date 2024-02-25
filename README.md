<h1 align="center">
numbers
</h1>

[![CMake CI Matrix](https://github.com/guuzaa/numbers/actions/workflows/cmake.yml/badge.svg?branch=main)](https://github.com/guuzaa/numbers/actions/workflows/cmake.yml)

`numbers` is a library for C++17 and later versions that handles integer overflow similar to Rust. It simplifies integer computations and offers control over how to handle overflow situations.

## Features

- **Full Control**

- **Like Primitive Types**

- **Signed Integers**: int8 int16 int32 int64 int128

- **Unsigned Integers**: uint8 uint16 uint32 uint64

## How to use

### operator +
```c++
numbers::int8 a = 100;
std::cout << a << '\n';
try {
    a = a + a;
    std::cout << a << '\n';
} catch (std::runtime_error &err) {
    std::cout << "Catch error: " << err.what() << '\n';
}
```

### checked sub
```c++
numbers::int8 a = numbers::int8::MIN;
std::cout << a << '\n';
std::optional<numbers::int8> ret = a.checked_sub(1);
if (ret) {
    std::cout << ret.value() << '\n';
} else {
    std::cout << "Overflow!\n";
}
```

### overflowing div
```c++
numbers::int16 a = 40;
numbers::int16 b = 2;
auto [ret, overflowing] = a.overflowing_div(b);
std::cout <<"a= " << a << ", b= " << b << '\n';
if (!overflowing) {
    std::cout << ret << '\n';
} else {
    std::cout << "Overflow!\n";
}
```

### saturating mul
```c++
numbers::int64 a = 40;
numbers::int64 b = numbers::int64::MAX;
std::cout << "a= " << a << ", b= " << b << '\n';
numbers::int64 ret = a.saturating_mul(b);
std::cout << ret << '\n';
```