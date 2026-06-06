# Project 1 — "Sentinel": C/C++ Bug & Vulnerability Analysis Toolkit
### Project specification

> **How to use this file:** this is the source of truth for *what the project must do*. Work **one milestone at a time**, each on its own git branch, opening a PR per milestone. Do not start a milestone until the previous one's *acceptance criteria* pass.

---

## 1. Goal

Build a command-line toolkit that **finds bugs and security vulnerabilities in real C/C++ source code**, using:
- **Static analysis** — inspect source via the Clang AST (LibTooling + AST Matchers).
- **Dynamic analysis** — instrument programs at the LLVM IR level (an LLVM pass) to catch errors at runtime.
- **Fuzzing** — drive a target with libFuzzer + sanitizers to discover crashing inputs.

This mirrors the HTEC "AI, Compilers & System Software" internship domain (*"static/dynamic analysis tools for detecting bugs and security vulnerabilities"*). The deliverable is a usable tool plus a report of real findings on an open-source target.

---

## 2. Environment & stack

- **Language:** C++17.
- **Build:** CMake ≥ 3.16 + Ninja or Make.
- **Compiler / libraries:** LLVM + Clang **18** development packages.
- **Testing:** GoogleTest (fetched via CMake `FetchContent`).
- **Sanitizers:** AddressSanitizer + UndefinedBehaviorSanitizer (ship from Clang).
- **OS:** Linux (Ubuntu 24.04) or WSL2. Also works on a Colab CPU runtime.

Setup commands to put in `scripts/setup.sh` and document in the README:

```bash
sudo apt-get update
sudo apt-get install -y llvm-18-dev libclang-18-dev clang-18 clang-tools-18 \
                        cmake ninja-build git
export LLVM_DIR=/usr/lib/llvm-18/lib/cmake/llvm
export Clang_DIR=/usr/lib/llvm-18/lib/cmake/clang
```

CMake must locate LLVM/Clang via `find_package(LLVM 18 REQUIRED CONFIG)` and `find_package(Clang REQUIRED CONFIG)`.

---

## 3. Target repository layout

Produce roughly this structure:

```
sentinel/
├── README.md
├── CMakeLists.txt
├── scripts/
│   ├── setup.sh                # install toolchain
│   └── run_on_target.sh        # run the full toolkit on a chosen repo
├── src/
│   ├── static/                 # Milestone 2: Clang LibTooling analyzer
│   │   ├── main.cpp            # CLI entry (CommonOptionsParser)
│   │   ├── Rules.h / Rules.cpp # AST-matcher rules + diagnostics
│   ├── pass/                   # Milestone 3: LLVM instrumentation pass (plugin)
│   │   └── BoundsPass.cpp      # New Pass Manager plugin
│   ├── fuzz/                   # Milestone 4: libFuzzer harnesses
│   │   └── parse_fuzzer.cpp
│   └── cli/                    # Milestone 5: orchestrator
│       └── sentinel.cpp
├── tests/
│   ├── static_tests.cpp        # GoogleTest: rules fire on bad code, quiet on good
│   ├── fixtures/               # tiny .c/.cpp files with planted bugs + clean ones
│   └── pass_tests/             # programs the pass should trap at runtime
├── targets/                    # small open-source C target(s) to analyze (submodule or vendored)
└── docs/
    └── findings.md             # Milestone 5: real bugs found, with repro
```

---

## 4. Global engineering rules (apply to every milestone)

1. Every executable builds with `-Wall -Wextra -g` and **zero warnings**.
2. Every milestone adds tests; `ctest` must pass before the milestone is "done".
3. Provide a `--help` and clear `file:line: [rule] message` diagnostic format for all findings.
4. No milestone may break a previous one. Keep `main` green.
5. Update `README.md` each milestone: how to build, run, and what's implemented.
6. Prefer real inputs: the `tests/fixtures/` set proves correctness, but the tool must also run on the real code in `targets/`.

---

## 5. Milestones

### Milestone 1 — Toolchain + first AST walk
**Objective:** working build that can parse a real C/C++ file and walk its Clang AST.
**Tasks:**
- Scaffold the repo, `CMakeLists.txt`, `scripts/setup.sh`, README skeleton.
- A LibTooling tool (`src/static/main.cpp`) using `CommonOptionsParser` that, given a source file, prints every function definition and every function call (name + `file:line`).
- Add a couple of fixture files under `tests/fixtures/`.
**Deliverable:** `sentinel-static <file.c>` prints the function/call inventory.
**Acceptance criteria:**
- `cmake -S . -B build && cmake --build build` succeeds with no warnings.
- `./build/sentinel-static tests/fixtures/sample.c -- -std=c11` lists ≥1 function and ≥1 call with correct line numbers.

### Milestone 2 — Static analyzer with AST matchers
**Objective:** detect concrete bug/vulnerability patterns in source.
**Tasks (implement each as an AST-matcher rule, emitting a diagnostic):**
- R1: calls to unsafe functions (`gets`, `strcpy`, `strcat`, `sprintf`).
- R2: floating-point equality (`==`/`!=` between floating operands).
- R3: assignment used as a condition (`if (x = y)`).
- R4: `malloc`/`new` whose result is not freed within the same function scope (best-effort intra-procedural).
- Wire all rules behind flags (`--enable=R1,R3` etc.; default all on).
- GoogleTest: each rule fires on a *bad* fixture and stays silent on a *clean* fixture.
**Deliverable:** `sentinel-static` reports findings for the 4 rules on real code.
**Acceptance criteria:**
- `ctest` green: every rule has a positive and a negative test.
- Running on `targets/` produces a non-empty, correctly-formatted findings list.

### Milestone 3 — Dynamic analysis: an LLVM instrumentation pass
**Objective:** catch a bug class at runtime by instrumenting IR.
**Tasks:**
- Implement a New Pass Manager **pass plugin** (`src/pass/BoundsPass.cpp`) that inserts checks for **one** class of error. Pick array/pointer bounds OR signed integer overflow. On violation, the instrumentation calls a runtime function that prints `runtime error: <kind> at <loc>` and aborts.
- Build it as a shared plugin loadable via `opt -load-pass-plugin` and via `clang -fpass-plugin=`.
- `tests/pass_tests/`: buggy programs that, once compiled with the pass, abort with the diagnostic; clean programs that run normally.
**Deliverable:** a `.so` pass plugin + a wrapper script that compiles a program with instrumentation.
**Acceptance criteria:**
- A planted out-of-bounds (or overflow) test program, compiled with the pass, **aborts with the expected message**; the matching clean program exits 0.

### Milestone 4 — Fuzzing: find real crashes
**Objective:** discover crashing inputs in a target via coverage-guided fuzzing.
**Tasks:**
- Write a libFuzzer harness (`src/fuzz/parse_fuzzer.cpp`) for a parser-like function (either a small bundled target with a planted bug, or a function from `targets/`).
- Build it with `clang -g -fsanitize=fuzzer,address,undefined`.
- Add a `corpus/` seed dir and a `scripts/fuzz.sh` that runs for a bounded time.
- When a crash is found, minimize it and write a root-cause note.
**Deliverable:** a fuzzer binary + at least one minimized crashing input + a short root-cause writeup.
**Acceptance criteria:**
- `scripts/fuzz.sh` finds a crash on the seeded buggy target within a bounded run (e.g. ≤ 60s) and saves the reproducer.

### Milestone 5 — Orchestrator CLI, real run, polish
**Objective:** one tool that runs static + dynamic over a target, plus a real report.
**Tasks:**
- `src/cli/sentinel.cpp`: a CLI that takes a path, runs the static rules across all sources, optionally builds-with-pass and/or fuzzes designated entry points, and prints a consolidated report (text + a machine-readable JSON option).
- Run it on a real open-source C project (vendored under `targets/`), capture findings in `docs/findings.md` with repro steps.
- Finalize README (architecture, usage, results) and a short design doc.
**Deliverable:** complete repo + `docs/findings.md` listing real findings + a 1–2 page design writeup.
**Acceptance criteria:**
- `sentinel <target-dir>` runs end-to-end and emits a consolidated report.
- `docs/findings.md` contains ≥1 genuine issue found in real code, with reproduction.

---

## 6. Definition of done (whole project)
- Clean build, zero warnings, `ctest` green.
- Static analyzer (≥4 rules), an LLVM instrumentation pass, and a working fuzzer, all integrated behind one CLI.
- Demonstrated on real code with a written findings report.
- README lets a newcomer build and run everything in under 10 minutes.

## 7. References (consult, do not copy)
- Clang **LibTooling** and **AST Matcher Reference** (official Clang docs).
- LLVM **"Writing an LLVM Pass" (New Pass Manager)** and the **Programmer's Manual**.
- **libFuzzer** documentation and the **AddressSanitizer/UBSan** docs.
