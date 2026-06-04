# Lesson 06 — Functions, `const` & headers

## Passing arguments: four options (this matters in C++)

In Python everything is passed by object-reference; in Java, primitives by value and objects by reference. In C++ **you choose**, and the choice affects both correctness and performance.

```cpp
void by_value(std::vector<int> v);        // makes a COPY (can be expensive)
void by_reference(std::vector<int>& v);   // alias; can modify the caller's object
void by_const_ref(const std::vector<int>& v); // alias; read-only, no copy  <-- common default
void by_pointer(std::vector<int>* v);     // alias via pointer; can be null
```

Guideline:
- Small, cheap types (`int`, `double`): pass **by value**.
- Big objects you only read: pass **by `const&`** (no copy, can't accidentally modify).
- Need to modify the caller's object: pass **by reference** (`&`).

## `const` means "I promise not to change this"

`const` is checked by the compiler. It documents intent and prevents whole classes of mistakes.

```cpp
int total(const std::vector<int>& xs);  // promises not to modify xs
const double PI = 3.14159;              // a constant value
```

## Headers (`.h`) vs. source (`.cpp`) — separate compilation

A `.cpp` is compiled on its own into a `.o`. So how does `main.cpp` know that a function defined in `mathutils.cpp` exists? You **declare** it in a header that both files share:

- **Header (`.h`)** — *declarations*: "this function exists, here is its signature." Included by anyone who wants to call it.
- **Source (`.cpp`)** — *definitions*: the actual function bodies. Compiled once.

The linker then connects the call in `main.o` to the body in `mathutils.o`. (An "undefined reference" link error usually means you forgot to compile/link the `.cpp` that has the body.)

Every header needs an **include guard** (or `#pragma once`) so it isn't pasted twice:

```cpp
#ifndef MATHUTILS_H
#define MATHUTILS_H
// ... declarations ...
#endif
```

This lesson is built from **two** source files (`mathutils.cpp` + the example/exercise), sharing `mathutils.h`. Look at how `CMakeLists.txt` lists both.

## Your task

`mathutils.h` declares `int clampi(int v, int lo, int hi);`. Implement it in `mathutils.cpp`, and complete the `by_const_ref` sum in `exercise.cpp`.
