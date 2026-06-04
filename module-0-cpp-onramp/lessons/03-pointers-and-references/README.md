# Lesson 03 — Pointers & references

> Go slow here. This is the lesson everything else builds on. If pointers click,
> the rest of C++ is mostly details.

**New ideas vs. Java/Python:** you can talk about a variable's **address**
directly. Two new operators:

- `&x` — "the **address of** x" (where it lives in memory).
- `*p` — "the **value at** the address p holds" (called *dereferencing*).

## Pointers

A pointer is just a variable whose value is an address.

```cpp
int x = 41;
int* p = &x;   // p holds the address of x
*p = 42;       // write through p -> x is now 42
int y = *p;    // read through p  -> y is 42
```

In Java/Python you already use references everywhere — but they're invisible and
you can't see or do arithmetic on the address. C++ makes the address a
first-class value. That power is why C/C++ can write operating systems and
compilers — and why it lets you create memory bugs (next lesson).

### Two ways a pointer goes wrong
- **Null pointer:** `int* p = nullptr;` points at nothing. Dereferencing it
  (`*p`) crashes. **Always check** `if (p != nullptr)` before dereferencing a
  pointer that might be null.
- **Dangling pointer:** a pointer to memory that no longer exists (e.g. a local
  variable whose function already returned, or heap memory you `delete`d). Using
  it is *undefined behavior* — sometimes it "works", sometimes it corrupts data,
  sometimes it crashes. Lesson 04 makes you cause one on purpose.

## References

A **reference** is an alias for an existing variable. Once bound, it *is* that
variable — no `*` to use it, and it can never be null or re-bound.

```cpp
int x = 10;
int& r = x;   // r is another name for x
r = 20;       // x is now 20
```

References are the clean, safe default for "let this function modify my
variable" or "don't copy this big object":

```cpp
void add_one(int& n) { n += 1; }   // caller's int is modified, no pointers
```

## Pointer vs. reference — when to use which

| | Pointer `T*` | Reference `T&` |
|--|--------------|----------------|
| Can be null? | yes | no |
| Can be re-aimed? | yes (`p = &other`) | no (bound once) |
| Syntax to use | `*p` | just `r` |
| Use it when… | it may be absent; you need to re-point; talking to C APIs | it's always present; you just want to avoid a copy |

Rule of thumb: **prefer references**; reach for a pointer only when you need
"maybe nothing" (null) or re-pointing.

## Build, run, do the exercise

```bash
g++ -std=c++17 -Wall -Wextra -g example.cpp -o example && ./example
g++ -std=c++17 -Wall -Wextra -g -I../../include exercise.cpp -o exercise && ./exercise
```

The exercise asks you to write through a pointer (with a null check), return a
reference to the larger of two values, and validate a pointer. Green when every
`CHECK` passes.
