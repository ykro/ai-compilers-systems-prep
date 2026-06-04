# Lesson 07 — Capstone: a dynamic array (with a bug to catch)

This is where it all comes together. You'll read a small **dynamic array** (a
tiny `std::vector`) built from raw heap memory, then **hunt a planted memory bug
with AddressSanitizer** — exactly the skill the rest of the internship is about.

## What's here

| File | Role |
|------|------|
| `dyn_array.h` | the interface: a `DynArray` class (header — Lesson 06) |
| `dyn_array.cpp` | the implementation with `new[]/delete[]` (Lesson 04) — **and one planted bug** |
| `main.cpp` | a small program that pushes values and prints them |
| `BUGHUNT.md` | step-by-step: find the bug with a sanitizer and fix it |

It ties together everything: pointers (03), heap + sanitizers (04), why we
prefer RAII (05), and headers/separate compilation (06).

## Build & run

```bash
g++ -std=c++17 -Wall -Wextra -g main.cpp dyn_array.cpp -o capstone
./capstone
```

It will likely print the correct sum — *but it contains undefined behavior.*
That's the lesson: a passing run does not mean a correct program.

## The bug hunt

Now make the bug visible with AddressSanitizer, read the report, and fix it:

```bash
g++ -std=c++17 -g -fsanitize=address,undefined main.cpp dyn_array.cpp -o capstone_asan
./capstone_asan
```

Follow **[`BUGHUNT.md`](BUGHUNT.md)** for the full walkthrough (it's an
off-by-one in the array-growth copy loop). When you're done, the ASan run is
clean and still prints `sum = 210`.

## Stretch goals (optional)

- Add bounds checking to `DynArray::at()` and have it report on out-of-range.
- Add a `pop_back()` and a `clear()`.
- Rewrite the class to hold a `std::vector<int>` internally and notice how the
  bug class disappears entirely — RAII and a vetted container did the work for
  you. That tension (raw control vs. safe abstractions) is the whole point.

## You're done with Module 0

You can now reason about the compile/link model, pointers, the stack and heap,
why memory bugs happen, how sanitizers expose them, and the modern safe tools.

➡️ Next: **Project 1 — "Sentinel"** (`../../project-1-sentinel/SPEC.md`) and
**Project 2 — "Lumen"** (`../../project-2-lumen/SPEC.md`).
