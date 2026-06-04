# Lesson 03 — Pointers & references

This is the lesson that makes C/C++ click. Take it slowly.

## The model

Every variable lives at an **address** in memory. A **pointer** is just a variable that stores an address.

- `&x` means "the address of `x`".
- `int* p = &x;` declares `p` as "a pointer to an int", holding x's address.
- `*p` means "the value at the address `p` points to" (this is *dereferencing*).

```cpp
int x = 42;
int* p = &x;     // p holds the address of x
std::cout << *p; // prints 42  (follow the pointer)
*p = 100;        // writes through the pointer; now x == 100
```

### Java/Python contrast
In Java/Python, a variable holding an object is *already* a hidden reference, and you can never see or do arithmetic on the address. In C/C++ the indirection is **explicit and visible**, and that power is exactly why things can go wrong.

## Three ways things break (and why Project 1 exists)

- **Null pointer:** `int* p = nullptr; *p;` → crash (segfault). Always check or guarantee non-null.
- **Dangling pointer:** a pointer to something that no longer exists (e.g. a local variable that went out of scope, or memory you already freed). Reading it is *undefined behavior*.
- **Pointer arithmetic off the end:** pointers and arrays are intertwined; `p[i]` is `*(p + i)`. Going out of range is a buffer overflow.

## References — a safer, simpler alias

A **reference** (`int&`) is an alias for an existing variable. It cannot be null and cannot be reseated. Prefer references over pointers when you just need to refer to something that definitely exists (very common in function parameters — Lesson 06).

```cpp
int x = 5;
int& r = x;   // r is another name for x
r = 7;        // x is now 7
```

## Your task

Implement the `TODO`s in `exercise.cpp`: a function that swaps two ints **through pointers**, and one that increments an int **through a reference**. This is the core skill for everything that follows.
