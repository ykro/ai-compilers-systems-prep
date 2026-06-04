# Bug Hunt — the capstone

`dyn_array.cpp` has **one planted memory bug**. This is a rehearsal for what
the internship's tools (Project 1 — Sentinel) automate: making an invisible bug
*visible*, then fixing it.

## Step 1 — Run it normally

```bash
g++ -std=c++17 -Wall -Wextra -g main.cpp dyn_array.cpp -o capstone
./capstone
```

It probably prints the right answer (`sum = 210`). That's the trap: the bug is
**undefined behavior**, and UB is allowed to *look* like it works. You can't
trust "it ran fine."

## Step 2 — Turn on AddressSanitizer

```bash
g++ -std=c++17 -g -fsanitize=address,undefined main.cpp dyn_array.cpp -o capstone_asan
./capstone_asan
```

Now it aborts with a report like:

```
==12345==ERROR: AddressSanitizer: heap-buffer-overflow on address 0x...
READ of size 4 at 0x... thread T0
    #0 ... DynArray::grow() dyn_array.cpp:XX
    #1 ... DynArray::push_back(int) dyn_array.cpp:XX
    #2 ... main main.cpp:XX
```

Read it top-to-bottom: **what** (heap-buffer-overflow), **how** (a READ of size
4 — a 4-byte `int`), and **where** (the exact file and line, inside
`DynArray::grow()`).

## Step 3 — Diagnose

ASan points at the copy loop in `grow()`:

```cpp
for (std::size_t i = 0; i <= size_; ++i) {
    new_data[i] = data_[i];
}
```

`grow()` is called when `size_ == cap_`. The old buffer `data_` only has
`cap_` elements, so its valid indices are `0 .. cap_-1`. The `<=` makes the
loop read `data_[size_]` (= `data_[cap_]`) — **one past the end**. Classic
off-by-one.

## Step 4 — Fix it

Change `<=` to `<`:

```cpp
for (std::size_t i = 0; i < size_; ++i) {
    new_data[i] = data_[i];
}
```

Rebuild with ASan and confirm the run is now **clean** (no report) and still
prints `sum = 210`:

```bash
g++ -std=c++17 -g -fsanitize=address,undefined main.cpp dyn_array.cpp -o capstone_asan
./capstone_asan
```

## What you just practiced

- A bug that's invisible to the naked eye is obvious to a sanitizer.
- The report's stack trace takes you to the precise line — no guessing.
- Off-by-one (`<=` vs `<`) and buffer overflows are among the most common —
  and most exploitable — bugs in real C/C++ code.

That's the entire premise of the internship track: **build the tools that find
these automatically.** You're ready for Project 1 — Sentinel.
