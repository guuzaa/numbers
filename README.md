<h1 align="center">
numbers
</h1>

[![CMake CI Matrix](https://github.com/guuzaa/numbers/actions/workflows/cmake.yml/badge.svg?branch=main)](https://github.com/guuzaa/numbers/actions/workflows/cmake.yml)
![language c++17](https://img.shields.io/badge/Language-C++17-red)
[![license mit](https://img.shields.io/badge/License-MIT-pink)](https://github.com/guuzaa/numbers/blob/main/LICENSE.txt)
![linux](https://img.shields.io/badge/OS-Linux-blue)
![macOS](https://img.shields.io/badge/OS-macOS-blue)
![windows](https://img.shields.io/badge/OS-Windows-blue)

> [!IMPORTANT]  
> This project is in the early stages of development. The codebase is subject to significant changes and reorganization. Expect breaking changes as we refine the architecture, fix bugs, and implement new features.

`numbers` is a library for C++17 and later versions that handles integer overflow similar to Rust. It simplifies integer overflow situations.

## Features

- **Full Control** over handling integer overflow

- Same as **Primitive Types** (WIP)

- **Support Integers**: i8, i16, i32, i64, u8, u16, u32, u64, even i128 & u128

## Usage

### operator +
```c++
numbers::i8 a = 100;
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
numbers::i8 a = numbers::i8::MIN;
std::cout << a << '\n';
std::optional<numbers::i8> ret = a.checked_sub(1);
if (ret) {
    std::cout << ret.value() << '\n';
} else {
    std::cout << "Overflow!\n";
}
```

### overflowing div
```c++
numbers::i16 a = 40;
numbers::i16 b = 2;
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
numbers::i64 a = 40;
numbers::i64 b = numbers::i64::MAX;
std::cout << "a= " << a << ", b= " << b << '\n';
numbers::i64 ret = a.saturating_mul(b);
std::cout << ret << '\n';
```

## Contribute

We welcome contributions, but please be aware that the project's design and conventions are still evolving. If you'd like to contribute, it's a good idea to discuss your plans with the project maintainers before starting work.

For the latest updates and discussions, please see our [issues](https://github.com/guuzaa/numbers/issues) and [pull requests](https://github.com/guuzaa/numbers/pulls).

Stay tuned for more updates, and thank you for your interest in contributing to our project!
