# C/C++ Onramp

A short, hands-on bridge into **C and C++** for students coming from **Java, Python, or Flutter/Dart**.

This is **Module 0** of the HTEC internship prep track ("AI, Compilers & System Software"). It exists so that nobody gets stuck on pointers, memory, or the compile/link model during Week 1 of the real projects. It is meant to take **about one week** (6 short lessons + 1 capstone).

## Why this module exists

In Java and Python the runtime manages memory for you, and "a variable that points to an object" is the only model you ever see. C and C++ hand you the memory directly. That is exactly *why* the internship team works on compilers, debuggers, and tools that **find memory bugs and security vulnerabilities** — those bugs only exist because the language lets you manage memory yourself. To build those tools, you first need to feel the bugs in your own hands.

By the end you will be comfortable with: the compile/link model, value vs. reference semantics, pointers, the stack vs. the heap, why memory bugs happen, RAII and smart pointers, and the tooling (`g++`/`clang++`, a debugger, and **sanitizers**).

## Lessons

| # | Topic | What's new vs. Java/Python |
|---|-------|----------------------------|
| 01 | Setup & the compilation model | There is no "run" button: source → compile → link → binary |
| 02 | Variables, types & the stack | Fixed-size integers that **overflow**; values are copied, not referenced |
| 03 | Pointers & references | Explicit addresses; `*` and `&`; null and dangling pointers |
| 04 | Memory: the heap & why bugs happen | `new`/`delete`, leaks, use-after-free, buffer overflow + **sanitizers** |
| 05 | RAII & smart pointers | Destructors, `std::unique_ptr`, `std::vector`, `std::string` — the modern, safe way |
| 06 | Functions, `const` & headers | Pass by value/reference/`const&`/pointer; `.h` vs `.cpp`; separate compilation |
| 07 | Capstone: a dynamic array (with a bug to catch) | Put it together, then hunt a real memory bug with AddressSanitizer |

Each lesson folder has a `README.md` (the concept), an `example.cpp` (a complete, runnable demo), and an `exercise.cpp` (your turn — it compiles as shipped, but the `CHECK` assertions fail until you implement the `TODO`s).

## How to build and run

### Option A — Locally (recommended, so you practice a real IDE + debugger)
You need a C++ compiler (`g++` or `clang++`) and CMake.

```bash
cmake -S . -B build
cmake --build build
# run any program, e.g.:
./build/lesson02_example
./build/lesson07_capstone
```

Open the folder in **CLion** or **VS Code + the clangd extension** for autocompletion, inline errors, and a debugger.

### Option B — On Google Colab (no install needed)
See [`colab/QUICKSTART.md`](colab/QUICKSTART.md). In short: compile single files with `!g++ file.cpp -o prog && ./prog`.

## Running with sanitizers (do this from Lesson 04 onward)

Sanitizers are extra checks the compiler bakes into your program to catch memory bugs at runtime. This is the heart of Project 1.

```bash
g++ -g -fsanitize=address,undefined lessons/04-memory-and-bugs/example.cpp -o demo
./demo
```

ASan prints an exact report (file, line, kind of error) the moment a bug happens.

## Suggested pace (1 week)

- Day 1: Lessons 01–02
- Day 2: Lesson 03 (pointers — go slow here)
- Day 3: Lesson 04 (memory + sanitizers)
- Day 4: Lesson 05 (RAII / smart pointers)
- Day 5: Lesson 06 + start the capstone
- Day 6: Finish the capstone and the bug hunt

When this feels comfortable, you are ready for **Project 1 — "Sentinel"**.
