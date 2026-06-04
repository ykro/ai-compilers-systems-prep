# Lesson 04 — Memory: the heap & why bugs happen

This is the heart of the onramp. Everything the internship's analysis tools do
exists because of the bugs in this lesson.

**New ideas vs. Java/Python:** you allocate memory yourself with `new`, and you
must hand it back with `delete`. There is no garbage collector. Get it wrong and
you get the famous memory bugs — which you'll now cause on purpose and then
catch with **sanitizers**.

## Stack vs. heap

| | Stack | Heap |
|--|-------|------|
| Lifetime | until the function returns | until **you** `delete` it |
| Who manages it | the compiler, automatically | **you** |
| Size | small, fixed | large |
| How | just declare a local | `new` / `new[]` … `delete` / `delete[]` |

```cpp
int* p = new int(5);     // one int on the heap
delete p;                // give it back

int* a = new int[10];    // array on the heap
delete[] a;              // note: delete[] for arrays
```

Pair them correctly: `new` ↔ `delete`, `new[]` ↔ `delete[]`.

## The four bugs to know by name

1. **Leak** — you `new` something and never `delete` it. Memory is lost until
   the process exits.
2. **Use-after-free** — you `delete` memory, then read or write it through a
   now-**dangling** pointer.
3. **Double free** — you `delete` the same pointer twice.
4. **Buffer overflow** — you read or write past the end of an array
   (`a[10]` on a 10-element array).

All four are *undefined behavior*: the program might crash, might silently
corrupt data, might appear to work. That unpredictability is exactly why they're
dangerous and why we need tools to find them.

## Sanitizers — your X-ray glasses

A **sanitizer** is extra checking the compiler bakes into your binary. Turn it
on with one flag and the program aborts at the *exact* line a bug happens, with
a precise report.

```bash
# Compile example.cpp with a bug enabled AND AddressSanitizer:
g++ -std=c++17 -g -DSHOW_BUG=1 -fsanitize=address,undefined example.cpp -o demo
./demo
```

`SHOW_BUG=1` triggers a buffer overflow, `=2` a use-after-free, `=3` a leak.
Run each and read the report — note how it names the file, the line, and the
kind of error.

- **ASan** (`-fsanitize=address`) catches overflows, use-after-free, leaks.
- **UBSan** (`-fsanitize=undefined`) catches signed overflow, bad shifts, etc.
- Always pair them with `-g` so the report has source lines.

> Run sanitizers from this lesson onward, on everything. They cost some speed
> but find bugs you'd otherwise ship.

## The exercise

`exercise.cpp` asks you to allocate a heap array, fill and sum it without
overflowing, and free it. First make the `CHECK`s pass, then **run the same
program under `-fsanitize=address`** and confirm there's no report — proof your
allocation is leak-free and in-bounds.

```bash
g++ -std=c++17 -Wall -Wextra -g -I../../include exercise.cpp -o exercise && ./exercise
g++ -std=c++17 -g -fsanitize=address,undefined -I../../include exercise.cpp -o ex_asan && ./ex_asan
```

Next lesson: the modern way to never write `delete` by hand again — **RAII and
smart pointers**.
