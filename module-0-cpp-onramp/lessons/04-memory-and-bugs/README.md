# Lesson 04 — Memory: the heap & why bugs happen

This is where the internship's whole problem domain comes from. Read carefully.

## Stack vs. heap

- **Stack:** automatic storage for local variables. Created when a function is entered, destroyed when it returns. Fast, automatic, but its lifetime is tied to the scope.
- **Heap:** memory you request explicitly and that lives until you release it. In C you use `malloc`/`free`; in C++ you use `new`/`delete`. **You** are responsible for releasing it.

```cpp
int* p = new int(7);   // allocate one int on the heap, value 7
// ... use *p ...
delete p;              // release it. Forgetting this = a leak.

int* arr = new int[100];   // an array
delete[] arr;              // note the [] form for arrays
```

In Java/Python a garbage collector frees memory for you. In C/C++ there is no GC by default — and that is the source of an entire category of bugs and CVEs.

## The four classic memory bugs

| Bug | What it is |
|-----|------------|
| **Memory leak** | You `new` but never `delete`. The program slowly eats memory. |
| **Use-after-free** | You `delete` something, then keep using the old pointer (now *dangling*). |
| **Double free** | You `delete` the same pointer twice. |
| **Buffer overflow** | You read/write past the end of an array. The #1 security vulnerability class. |

These don't always crash immediately — that's what makes them dangerous and hard to find. They corrupt memory quietly.

## Your superpower: sanitizers

The compiler can instrument your program to catch these at the instant they happen. **AddressSanitizer (ASan)** and **UndefinedBehaviorSanitizer (UBSan)**:

```bash
g++ -std=c++17 -g -fsanitize=address,undefined -I../../include example.cpp -o demo
./demo
```

`example.cpp` contains a deliberate heap buffer overflow. Compile it **without** sanitizers and it may appear to "work" (scary!). Compile it **with** `-fsanitize=address` and ASan prints exactly where the overflow is. *Project 1 is, in part, about building tools like this yourself.*

## Your task

`exercise.cpp` has a function with a memory leak and a function with a buffer overflow. Fix both so the program runs cleanly under `-fsanitize=address`. (The `CHECK`s confirm behavior; ASan confirms memory-cleanliness.)
