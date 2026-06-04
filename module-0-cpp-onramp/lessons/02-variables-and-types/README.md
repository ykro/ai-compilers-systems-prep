# Lesson 02 — Variables, types & the stack

## What's different from Java/Python

1. **Integers are fixed-size and they overflow silently.** Python's `int` grows without bound; Java throws nothing but at least the sizes are fixed and documented. In C++, an `int` is (usually) 32 bits, and `2'000'000'000 + 2'000'000'000` **overflows** — wrapping around to a negative number. This is a classic source of bugs and security holes (and a thing Project 1's tools will detect!). Prefer the explicit-width types from `<cstdint>` (`int32_t`, `uint64_t`, ...) when size matters.

2. **Most variables live on the stack and are copied by value.** When you write `b = a;` for an `int` or a `struct`, you get a **copy**, not a shared reference. This is the opposite of Java/Python, where assigning objects copies a reference. (Pointers and references, in Lesson 03, are how you opt back into sharing.)

3. **`auto` infers the type** (like `var` in Java, or just dynamic typing in Python) — but the type is still fixed at compile time.

4. **Uninitialized variables hold garbage.** `int x;` then reading `x` is *undefined behavior*, not "0". Always initialize.

## Key syntax

```cpp
#include <cstdint>

int        count   = 0;          // 32-bit signed (typically)
std::int64_t big   = 9'000'000'000;  // explicit 64-bit; ' is a digit separator
double     ratio   = 3.14;
bool       ok      = true;
char       letter  = 'A';        // a single byte, not a string
auto       n       = 42;         // deduced as int

const double PI = 3.14159;       // cannot be reassigned (like final / a constant)
```

## Your task

Open `exercise.cpp` and implement the two `TODO`s so the `CHECK`s pass. Build and run:

```bash
g++ -std=c++17 -Wall -Wextra -g -I../../include exercise.cpp -o ex && ./ex
```

You'll demonstrate that you understand (a) integer overflow and (b) value-copy semantics.
