# Lesson 05 — RAII & smart pointers (the modern, safe way)

Lesson 04 showed how manual `new`/`delete` causes bugs. Modern C++ mostly **avoids manual memory management** with a pattern called RAII.

## RAII: Resource Acquisition Is Initialization

The idea: tie a resource's lifetime to an object's lifetime. When the object goes out of scope, its **destructor** runs automatically and releases the resource. No manual `delete`, no leaks — even if an exception is thrown.

```cpp
struct Logger {
    Logger()  { std::cout << "open\n";  }   // constructor: acquire
    ~Logger() { std::cout << "close\n"; }    // destructor: release (runs automatically)
};

void f() {
    Logger log;          // "open"
    // ... do work ...
}                        // "close" prints here automatically, at end of scope
```

Java has `try-with-resources` and Python has `with`; C++ RAII is more general — it applies to *every* object, automatically.

## Smart pointers — RAII for heap memory

Instead of raw `new`/`delete`, use:

- **`std::unique_ptr<T>`** — owns a heap object; frees it automatically when it goes out of scope. There is exactly one owner. This is your default.
- **`std::shared_ptr<T>`** — reference-counted shared ownership (use only when you genuinely need it).

```cpp
#include <memory>
auto p = std::make_unique<int>(7);   // heap int, value 7
std::cout << *p;                     // use like a pointer
// no delete needed - freed automatically at end of scope
```

## And don't hand-roll containers either

For dynamic arrays and strings, use the standard library:

- **`std::vector<T>`** — a growable array (like Java's `ArrayList`, Python's `list`). Handles its own memory.
- **`std::string`** — a real string type. Stop using raw `char*` for text.

```cpp
#include <vector>
#include <string>
std::vector<int> v = {1, 2, 3};
v.push_back(4);                 // grows automatically
for (int x : v) { /* range-for, like Python's for-in */ }
std::string name = "Galileo";
```

Rule of thumb for modern C++: **raw `new`/`delete` should be rare.** Reach for `vector`, `string`, and `unique_ptr` first. You learned the manual way in Lesson 04 so you understand what these are doing under the hood — and so you can build tools that analyze code that *doesn't* use them.

## Your task

In `exercise.cpp`, rewrite a leaky raw-pointer function to use `std::unique_ptr` and `std::vector` so there is no manual memory management and nothing to leak.
