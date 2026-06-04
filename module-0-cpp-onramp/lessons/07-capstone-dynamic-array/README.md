# Lesson 07 — Capstone: a dynamic array (with a bug to catch)

You'll assemble everything: a class with a constructor/destructor (RAII), manual
heap memory (`new[]`/`delete[]`), pointers and indices, and `const` methods —
spread across a header and a source file.

`DynArray` is a hand-written, growable array of ints (a tiny `std::vector`).
It builds, and its functional tests pass. **But it contains one real memory bug.**
That's the point: tests passing does *not* mean the memory is correct. Tools that
find the bug anyway are exactly what the HTEC team builds.

Work through `BUGHUNT.md`.
