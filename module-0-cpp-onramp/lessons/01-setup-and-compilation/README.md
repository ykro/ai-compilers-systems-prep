# Lesson 01 — Setup & the compilation model

**New idea vs. Java/Python:** there is no "run" button. Source code becomes a
program through distinct steps: **source → compile → link → binary**, and only
then do you run the binary.

## The mental model

| Stage | Tool | Input → Output | Java/Python analogy |
|-------|------|----------------|---------------------|
| Compile | `g++`/`clang++` | `.cpp` → `.o` (object file, machine code) | `javac` → `.class` |
| Link | the linker (invoked by `g++`) | `.o` files + libraries → one executable | (the JVM does this at load time) |
| Run | the OS | executable → a running process | `java Hello` / `python x.py` |

The big difference: the output is a **native binary**. There's no interpreter
or VM standing between your program and the CPU. That's why C/C++ is what you
use to build compilers, drivers, and performance-critical tools — and why the
mistakes are yours to make.

## Install a toolchain

You need a C++ compiler and (later) CMake.

- **macOS:** `xcode-select --install` gives you `clang++`. `brew install cmake`.
- **Ubuntu / WSL2:** `sudo apt-get install -y g++ cmake`.
- **No install?** Use [Google Colab](../../colab/QUICKSTART.md).

Check it works:

```bash
g++ --version      # or: clang++ --version
cmake --version
```

## Build and run `hello.cpp`

```bash
g++ -std=c++17 -Wall -Wextra -g hello.cpp -o hello
./hello
echo $?            # prints 0 — the exit code main() returned
```

What the flags mean:
- `-std=c++17` — use the C++17 standard (what this whole onramp targets).
- `-Wall -Wextra` — turn on warnings. **Read them.** Most early bugs are a
  warning you ignored.
- `-g` — include debug info, so a debugger can show you source lines later.
- `-o hello` — name the output binary `hello` (otherwise you get `a.out`).

## Try this

1. Delete the `;` after the `std::cout` line and recompile. Read the error —
   get used to what the compiler's voice sounds like.
2. Change `return 0;` to `return 3;`, recompile, run, then `echo $?`.
3. Compile **without** `-o`: `g++ hello.cpp`. Notice it produced `a.out`.

## Where this is going

The whole reason to feel the compile/link model is that the internship builds
**tools that operate on this pipeline** — analyzers that read your source, and
passes that instrument the compiled code. Lesson 04 and Project 1 — Sentinel
live right here.
