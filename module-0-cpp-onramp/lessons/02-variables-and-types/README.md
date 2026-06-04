# Lesson 02 — Variables, types & the stack

**New ideas vs. Java/Python:**
1. Integers are **fixed-size** and **overflow** (wrap around) instead of growing.
2. Assigning a value **copies** it — two variables are not the same object.
3. Local variables live on **the stack** and disappear when the function returns.

## Fixed-size integers

In Python an `int` grows without bound. In C++ a type has a fixed width:

| Type | Width | Range |
|------|-------|-------|
| `int32_t` | 32 bits | −2,147,483,648 .. 2,147,483,647 |
| `uint8_t` | 8 bits | 0 .. 255 |
| `int64_t` | 64 bits | ±9.2×10¹⁸ |
| `size_t` | platform (usually 64-bit) | 0 .. very large; used for sizes/indices |

Use `<cstdint>` for the sized names (`int32_t`, `uint8_t`, …). Prefer them when
the size matters.

### Overflow is a real bug class
Add past the maximum and the value **wraps around silently** — no exception,
no error:

```cpp
uint8_t x = 255;
x = x + 1;   // x is now 0
```

This is exactly the kind of mistake the analysis tools in Project 1 — Sentinel
are built to catch. Compile with `-fsanitize=undefined` (Lesson 04) and signed
overflow gets reported at runtime.

## Value semantics: assignment copies

```cpp
int a = 10;
int b = a;   // b is a COPY of a's value
b = 99;      // a is still 10
```

Coming from Java/Python this is the opposite of your instinct: there, assigning
an object makes another reference to the *same* object. Here, plain values are
copied bit-for-bit. (References, next lesson, are how you opt out of copying.)

## The stack

Local variables are allocated on **the stack** — a region of memory that grows
when you call a function and shrinks when it returns. You don't manage it: the
variable simply ceases to exist at the closing `}`. That's fast and automatic,
but it also means **you must never keep a pointer to a local after its function
returns** (Lesson 03/04 — dangling pointers).

## Build, run, do the exercise

```bash
# the worked demo:
g++ -std=c++17 -Wall -Wextra -g example.cpp -o example && ./example

# your turn (CHECKs fail until you fill in the TODOs):
g++ -std=c++17 -Wall -Wextra -g -I../../include exercise.cpp -o exercise && ./exercise
```

`exercise.cpp` asks you to (1) implement an 8-bit wrapping add and (2) write a
reference-based swap. When every `CHECK` prints `[PASS]`, you're done.
