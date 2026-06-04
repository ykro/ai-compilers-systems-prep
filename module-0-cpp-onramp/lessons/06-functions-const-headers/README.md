# Lesson 06 — Functions, `const` & headers

Two topics that make C++ programs more than one file: **how arguments are
passed**, and **how code is split across `.h`/`.cpp` and compiled separately**.

**New ideas vs. Java/Python:** you choose, per parameter, whether to copy,
alias, or take a pointer — and `const` lets you *promise* (and have the compiler
enforce) that you won't modify something. And there's no single "module" file:
declarations live in a header, definitions in a source file, joined by the
linker.

## The four ways to pass an argument

| Style | Signature | Copies? | Can modify caller? | Use when |
|-------|-----------|---------|--------------------|----------|
| By value | `f(int n)` | yes | no | small types; you want your own copy |
| By reference | `f(int& n)` | no | yes | you must modify the caller's variable |
| By const reference | `f(const T& v)` | no | no | reading a big object cheaply (**common default**) |
| By pointer | `f(int* p)` | no | yes (if non-null) | it may be absent (null); C-style APIs |

```cpp
void by_value(int n)            { n += 1; }   // caller unaffected
void by_reference(int& n)       { n += 1; }   // caller's int changes
long size_of(const std::vector<int>& v);      // no copy, read-only
void by_pointer(int* p)         { if (p) *p += 1; }   // check null first
```

Rule of thumb: **read big things by `const&`**, modify via `&`, use a pointer
only for "maybe nothing".

## `const`

`const` means "this won't change." It documents intent *and* the compiler
enforces it:

```cpp
const int limit = 100;          // can't reassign limit
void print(const std::string& s);  // print promises not to modify s
```

For function parameters, `const&` is the workhorse: cheap (no copy) and safe
(can't accidentally mutate the caller's data).

## Headers (`.h`) vs. sources (`.cpp`) — separate compilation

- A **header** declares *what* exists (function signatures, types). Other files
  `#include` it.
- A **source file** defines *how* it works.
- Each `.cpp` is compiled independently into an object file (`.o`); the
  **linker** stitches them into one binary.

This lesson has `mathutils.h` (declarations) and `mathutils.cpp` (definitions).
`example.cpp` includes the header and is compiled *separately*, then linked:

```bash
g++ -std=c++17 -Wall -Wextra -g example.cpp mathutils.cpp -o example
```

Two habits to keep:
- `#pragma once` at the top of every header (prevents double-inclusion).
- Put declarations in the `.h`, definitions in the `.cpp`. Change the `.cpp` and
  only it needs recompiling — that's why large projects build fast.

## The exercise

`exercise.cpp` (compiled together with `mathutils.cpp`) asks you to write a
reference modifier, a pointer modifier with a null check, and a `const&` reader
that counts elements above a threshold — then call into `mathutils` to prove the
header/source split links correctly.

```bash
g++ -std=c++17 -Wall -Wextra -g -I../../include exercise.cpp mathutils.cpp -o exercise && ./exercise
```

Next: the **capstone** — build a small dynamic array, then hunt a planted memory
bug with AddressSanitizer.
