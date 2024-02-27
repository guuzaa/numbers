<h1 align="center">
numbers
</h1>

[![CMake CI Matrix](https://github.com/guuzaa/numbers/actions/workflows/cmake.yml/badge.svg?branch=main)](https://github.com/guuzaa/numbers/actions/workflows/cmake.yml)
![Static Badge](https://img.shields.io/badge/Language-C++17-red)
![Static Badge](https://img.shields.io/badge/License-MIT-pink)
![Static Badge](https://img.shields.io/badge/OS-Linux-blue)
![Static Badge](https://img.shields.io/badge/OS-macOS-blue)

> [!IMPORTANT]  
> This project is in the early stages of development. The codebase is subject to significant changes and reorganization. Expect breaking changes as we refine the architecture, fix bugs, and implement new features.

`numbers` is a library for C++17 and later versions that handles integer overflow similar to Rust. It simplifies integer computations and offers control over how to handle overflow situations.

## Features

- **Full Control**

- **Like Primitive Types**

- **Signed Integers**: int8 int16 int32 int64 int128

- **Unsigned Integers**: uint8 uint16 uint32 uint64

## Usage

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

## Contribute

We welcome contributions, but please be aware that the project's design and conventions are still evolving. If you'd like to contribute, it's a good idea to discuss your plans with the project maintainers before starting work.

For the latest updates and discussions, please see our [issues](./issues) and [pull requests](./pulls).

Stay tuned for more updates, and thank you for your interest in contributing to our project!
