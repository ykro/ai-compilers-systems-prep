# Lesson 01 — Setup & the compilation model

## The one big mental shift

In **Python** you run `python script.py` and it executes. In **Java** you compile to bytecode that a JVM runs. In **C/C++ there is no runtime and no "run" button** — you turn source code into a native binary, then run that binary directly on the CPU.

The pipeline is:

```
source.cpp  --(preprocessor)-->  expanded source
            --(compiler)------>  object file  (.o)   one per .cpp
            --(linker)-------->  executable        combines all .o + libraries
```

Two practical consequences you'll feel right away:
- **Errors come in two flavors:** *compile errors* (syntax, types) and *link errors* (e.g. "undefined reference" = you declared a function but never provided its body, or forgot to compile the file that has it).
- **`#include` is literal text pasting.** A header is not an "import" of a compiled module like in Java/Python; the preprocessor copy-pastes the header's text into your file before compiling.

## Compile and run by hand

```bash
g++ -std=c++17 -Wall -Wextra -g hello.cpp -o hello
./hello
```

What the flags mean:
- `-std=c++17` — use modern C++ (we use C++17 in this module).
- `-Wall -Wextra` — turn on warnings. **Treat warnings as bugs you haven't hit yet.**
- `-g` — include debug info so a debugger can show source lines.
- `-o hello` — name the output binary.

## Tooling to set up now

- A compiler: `g++` or `clang++` (the internship team lives in the Clang/LLVM world, so `clang++` is great to have).
- **CMake** — the build system. Real C++ projects are not compiled by hand; CMake generates the build. See the root `CMakeLists.txt`.
- An IDE: **CLion**, or **VS Code + the `clangd` extension**. Both give you autocomplete, inline errors, and a debugger.
- A debugger: `gdb` (with g++) or `lldb` (with clang). Learn to set a breakpoint and step — it's a superpower.

## Your task

Read `hello.cpp`, compile it, run it. Then **break it on purpose**: delete a semicolon and read the compiler error; remove the `return 0;`? (it's optional in `main`). Add `int x = "hello";` and read the type error. Getting fluent at *reading compiler errors* is half the battle.
