<h1 align="center">
numbers
</h1>

`numbers` is a library for C++17 and later versions that handles integer overflow similar to Rust. It simplifies integer computations and offers control over how to handle overflow situations.

## Features

- **Full Control**

- **Like Primitive Types**

- **Signed Integers**: Int8 Int16 Int32 Int64

- **Unsigned Integers**: Uint8 Uint16 Uint32 Uint64

## How to use

### operator +
```c++
Int8 a = 100_i8;
std::cout << a << '\n';
try {
    a = a + a;
    std::cout << a << '\n';
} catch (std::runtime_error err) {
    std::cout << "Catch error: " << err.what() << '\n';
}
```

### checked sub
```c++
Int8 a = Int8::MIN;
std::cout << a << '\n';
std::optional<Int8> ret = a.checked_sub(1);
if (ret) {
    std::cout << ret.value() << '\n';
} else {
    std::cout << "Overflow!\n";
}
```

### overflowing div
```c++
Int16 a = 40_i16;
Int16 b = 2_i16;
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
Int64 a = 40_i64;
Int64 b = Int64::MAX;
std::cout << "a= " << a << ", b= " << b << '\n';
Int64 ret = a.saturating_mul(b);
std::cout << ret << '\n';
```