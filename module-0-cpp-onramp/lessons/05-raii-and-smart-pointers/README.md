# Lesson 05 — RAII & smart pointers

Lesson 04 showed you the bugs that come from managing memory by hand. This
lesson shows you the modern C++ answer: **don't manage it by hand.** Tie every
resource to an object's lifetime and let the compiler clean up for you.

**New ideas vs. Java/Python:** destructors run *deterministically* (the moment
an object leaves scope, not "sometime later" like a GC), and owning types
(`unique_ptr`, `vector`, `string`) free their memory automatically.

## RAII — the core idea

**RAII = Resource Acquisition Is Initialization.** A class grabs a resource in
its **constructor** and releases it in its **destructor**. Because the
destructor runs automatically when the object goes out of scope, the resource is
*always* released — even if you `return` early or an exception is thrown.

```cpp
struct File {
    FILE* f;
    File(const char* path)  { f = std::fopen(path, "r"); }  // acquire
    ~File()                 { if (f) std::fclose(f); }       // release, always
};
// When a File goes out of scope, the file is closed. You can't forget.
```

This is the opposite of Java's `finally` / Python's `with`: instead of
remembering to clean up at each call site, the *type* guarantees cleanup once.

## Smart pointers: `std::unique_ptr`

A `unique_ptr` owns a heap object and `delete`s it for you. It's the
RAII wrapper for `new`. Create it with `std::make_unique`:

```cpp
std::unique_ptr<int> p = std::make_unique<int>(42);
*p = 43;                 // use it like a pointer
// no delete needed — freed automatically when p goes out of scope
```

"Unique" means exactly one owner; you can `std::move` ownership but not copy it.
(There's also `std::shared_ptr` for shared ownership — reach for it only when you
truly need it.)

## Owning containers: `std::vector` and `std::string`

You almost never need `new[]` again:

- **`std::vector<T>`** — a growable array that owns its memory. `push_back`,
  `size()`, `[]`, range-`for`. Replaces manual `new T[]`.
- **`std::string`** — owns and grows a sequence of characters. Replaces manual
  `char` buffers (the source of countless overflows).

```cpp
std::vector<int> v = {1, 2, 3};
v.push_back(4);
for (int x : v) { /* ... */ }     // like `for x in v:` in Python

std::string s = "abc";
s += "def";                        // grows automatically
```

## The rule of thumb

> Prefer `vector`/`string`/`unique_ptr`. Write `new`/`delete` only when you're
> *implementing* a data structure (next lesson's capstone) — and even then, wrap
> it in RAII so callers never see a raw `delete`.

## The exercise

`exercise.cpp` has you build a `vector`, sum it, box an int in a `unique_ptr`,
and assemble a `string` — all with **zero** `new`/`delete`. Make the `CHECK`s
pass, then run under ASan and confirm there are no leaks: RAII gave you that for
free.

```bash
g++ -std=c++17 -Wall -Wextra -g -I../../include exercise.cpp -o exercise && ./exercise
g++ -std=c++17 -g -fsanitize=address -I../../include exercise.cpp -o ex_asan && ./ex_asan
```
