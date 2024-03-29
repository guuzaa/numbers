<h1 align="center">
numbers
</h1>

[![CMake CI Matrix](https://github.com/guuzaa/numbers/actions/workflows/cmake.yml/badge.svg?branch=main)](https://github.com/guuzaa/numbers/actions/workflows/cmake.yml)
![language c++17](https://img.shields.io/badge/Language-C++17-red)
[![license mit](https://img.shields.io/badge/License-MIT-pink)](https://github.com/guuzaa/numbers/blob/main/LICENSE.txt)

`numbers` is a library for C++17 and later versions that handles integer overflow similar to Rust. It simplifies integer overflow situations.

## Features

- **Full Control** over handling integer overflow

- **Support for Multiple Toolchains**: GCC, Clang, MSVC

- **Support for Various Integer Type**: i8, i16, i32, i64, u8, u16, u32, u64, even i128 & u128

## Examples

<details>
<summary>Show More</summary>

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

### wrapping add
```c++
numbers::u128 max = numbers::u128::MAX;
numbers::u128 ret = max.wrapping_add(1); // wrapping around
std::cout << ret << '\n';
```
</details>

## How to build
<details>
<summary>Show More</summary>

### Prerequisite

Make sure that `CMake` and `GCC`/`Clang`/`MSVC` are installed on your machine.

The source code, example code and test code are located in the [`src`](https://github.com/guuzaa/numbers/tree/main/src), [`examples`](https://github.com/guuzaa/numbers/tree/main/examples) and [`tests`](https://github.com/guuzaa/numbers/tree/main/tests) directory, respectively. 

### Generate build recipe

```shell
cmake -Bbuild
# If you are keen on Ninja
cmake -B build -G Ninja
```

### Build and run all examples

```shell
cmake --build build -t example
```

#### Build and run an exact example

```shell
cmake --build build -t example-[filename]
# If you want to run the file examples/hash.cc 
cmake --build build -t example-hash
# If you want to run a new file you are writing in the ./examples
cmake --build build -t example-your-file-name-with-no-extensions
```

### Build and run all tests

```shell
cmake --build build -t run-tests
```

There are two test binaries: integer_test, uinteger_test. Type the following commands to run them:

```shell
cmake --build build -t test-integer
cmake --build build -t test-uinteger
```

### Format code

> [!CAUTION]
> It requires that your machine has `clang-format` installed

```shell
cmake --build build -t check-format
cmake --build build -t format
```

</details>

## Contribute

If you'd like to contribute, it's a good idea to discuss your plans with the project maintainers before starting work.

For the latest updates and discussions, please see our [issues](https://github.com/guuzaa/numbers/issues) and [pull requests](https://github.com/guuzaa/numbers/pulls).

Stay tuned for more updates, and thank you for your interest in contributing to our project!
