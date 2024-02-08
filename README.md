<h1 align="center">
numbers
</h1>

numbers is a library handling integer overflow for C++17 and onwards. It allows for effortless integer computations and provides control over how to handle overflow situations.

## Features

- **Full Control**

- **Like Primitive Types**

## How to use

```c++
auto a = 100_i8;
std::cout << a << '\n';
try {
    a = a + a;
    std::cout << a << '\n';
} catch (std::runtime_error err) {
    std::cout << "Catch error: " << err.what() << '\n';
}
```
