# Project 1 — "Sentinel": C/C++ Bug & Vulnerability Analysis Toolkit
### Project specification (a guided learning project)

> **How to use this file:** this document is the complete source of truth for *what to
> build and how to know it works*. It is written so you can do the whole project from
> it alone, with no implementation handed to you — the source code is yours to write.
>
> Work **one milestone at a time**, each on its own git branch in your fork. **Do not
> start a milestone until the previous one's *Validation* checklist fully passes.** Keep
> `main` always building. The fork/branch/submit workflow is in
> [`CONTRIBUTING.md`](../CONTRIBUTING.md) — each milestone is submitted by email (repo
> link + branch).

---

## 1. What you are building (and why)

A command-line toolkit that **finds bugs and security vulnerabilities in real C/C++
source code**. You will build it three ways, each representing a major technique the
software-security industry actually uses:

- **Static analysis** — read the code *without running it*, by inspecting the **Clang
  AST** (the compiler's structured view of the source). You flag dangerous patterns.
- **Dynamic analysis** — *change the program as it is compiled* so it checks itself at
  runtime. You write an **LLVM instrumentation pass** that injects safety checks.
- **Fuzzing** — *run the program on millions of random inputs* to make it crash, using
  **libFuzzer** + sanitizers, then study the crash.

This mirrors the internship domain — *"static/dynamic analysis tools for detecting bugs
and security vulnerabilities."* The final deliverable is a usable tool **plus a written
report of real findings** on an open-source target.

### Why these three? — the compiler pipeline

A C/C++ program goes through several stages before it runs. Each analysis technique
attaches at a different stage. Understanding this picture is the key mental model for
the whole project:

```
  ── How a C/C++ program is built, and where Sentinel plugs in ──

     foo.c
       │
       ▼
   ┌──────────────┐   AST    ┌─────────────┐  LLVM IR  ┌───────────────┐   binary
   │ Clang        │ ───────► │ Clang       │ ────────► │ opt + backend │ ─────────►  ./a.out
   │ frontend     │          │ CodeGen     │           │ (passes)      │
   └──────────────┘          └─────────────┘           └───────────────┘
       ▲                                                   ▲                          ▲
       │                                                   │                          │
  (M2) STATIC ANALYSIS                            (M3) DYNAMIC ANALYSIS        (M4) FUZZING
  LibTooling + AST matchers                       an LLVM pass rewrites the    run a sanitizer-built
  walk the tree and flag risky                    IR to insert runtime         binary on random inputs
  patterns. No execution.                         checks (bounds / overflow).  until it crashes, then
  Fast, but only sees the code.                   Caught the moment you run.    minimize the input.
```

- **Static** sees *everything* in the source but can't know runtime values → false
  positives/negatives.
- **Dynamic** sees *exactly* what happened at runtime but only on the paths you actually
  execute.
- **Fuzzing** is how you *generate* those runtime paths automatically.

They are complementary. Milestone 5 ties all three behind one CLI:

```
  ── Sentinel toolkit (what the Milestone-5 CLI orchestrates) ──

                        ┌────────────────────────── sentinel (CLI) ──────────────────────────┐
                        │                                                                     │
  target/ (C/C++ src) ─►│   ┌───────────────┐   ┌────────────────────┐   ┌─────────────────┐ │─► report (text/JSON)
                        │   │ static engine │   │ dynamic engine     │   │ fuzz engine     │ │
                        │   │ (M2)          │   │ (M3)               │   │ (M4)            │ │─► docs/findings.md
                        │   │ Clang AST     │   │ build w/ pass →    │   │ libFuzzer +     │ │
                        │   │ matchers      │   │ instrumented binary│   │ sanitizers      │ │
                        │   └───────────────┘   └────────────────────┘   └─────────────────┘ │
                        └─────────────────────────────────────────────────────────────────────┘
```

---

## 2. Prerequisites & environment

You should be comfortable with the material in **Module 0** (pointers, the heap, the
four classic memory bugs, sanitizers, separate compilation). If any of that is shaky,
finish Module 0 first.

| Component | Requirement |
|-----------|-------------|
| Language  | C++17 |
| Build     | CMake ≥ 3.16 + Ninja (or Make) |
| Toolchain | LLVM + Clang **18** development packages |
| Testing   | GoogleTest (fetched via CMake `FetchContent`) |
| Sanitizers| AddressSanitizer + UndefinedBehaviorSanitizer (ship with Clang) |
| OS        | Linux (Ubuntu 24.04) or WSL2. A Colab CPU runtime also works. |

> **Why LLVM 18 specifically?** The LibTooling and Pass-plugin C++ APIs change between
> major LLVM versions. Pinning to 18 means the API names in this spec match what you
> have installed. If you use a different version, expect to adjust some include paths
> and class names.

### 2.1 Setup script

Put these commands in `scripts/setup.sh` and document them in your README:

```bash
#!/usr/bin/env bash
set -euo pipefail
sudo apt-get update
sudo apt-get install -y llvm-18-dev libclang-18-dev clang-18 clang-tools-18 \
                        cmake ninja-build git
# Tell CMake where LLVM/Clang's config packages live:
export LLVM_DIR=/usr/lib/llvm-18/lib/cmake/llvm
export Clang_DIR=/usr/lib/llvm-18/lib/cmake/clang
echo "LLVM_DIR=$LLVM_DIR"
echo "Clang_DIR=$Clang_DIR"
```

In your top-level `CMakeLists.txt`, locate the toolchain with:

```cmake
find_package(LLVM 18 REQUIRED CONFIG)
find_package(Clang REQUIRED CONFIG)
```

### 2.2 ✅ Validate your setup *before Milestone 1*

Run each and confirm the expected result. Don't proceed until all four pass.

```bash
clang-18 --version              # expect: "clang version 18.x.x"
llvm-config-18 --version        # expect: "18.x.x"
echo 'int main(){return 0;}' > /tmp/t.c && clang-18 /tmp/t.c -o /tmp/t && /tmp/t && echo OK   # prints OK
ls /usr/lib/llvm-18/lib/cmake/clang/ClangConfig.cmake   # file must exist (CMake needs it)
```

---

## 3. Repository layout

Produce roughly this structure (you create every file; nothing is provided):

```
sentinel/
├── README.md                   # how to build, run, and what's implemented (update each milestone)
├── CMakeLists.txt
├── scripts/
│   ├── setup.sh                # install the toolchain (section 2.1)
│   ├── build_with_pass.sh      # M3: compile a program with the instrumentation pass
│   ├── fuzz.sh                 # M4: run a fuzzer for a bounded time
│   └── run_on_target.sh        # M5: run the full toolkit on a chosen repo
├── src/
│   ├── static/                 # M2: Clang LibTooling analyzer
│   │   ├── main.cpp            #   CLI entry (CommonOptionsParser)
│   │   └── Rules.h / Rules.cpp #   AST-matcher rules + diagnostics
│   ├── pass/                   # M3: LLVM instrumentation pass (shared plugin)
│   │   ├── BoundsPass.cpp      #   New Pass Manager plugin
│   │   └── runtime.c           #   the check function the pass calls into
│   ├── fuzz/                   # M4: libFuzzer harnesses
│   │   └── parse_fuzzer.cpp
│   └── cli/                    # M5: orchestrator
│       └── sentinel.cpp
├── tests/
│   ├── static_tests.cpp        # GoogleTest: rules fire on bad code, quiet on good
│   ├── fixtures/               # tiny .c/.cpp files: planted bugs + clean ones
│   └── pass_tests/             # programs the pass should trap (or not) at runtime
├── targets/                    # small open-source C target(s) to analyze (vendored or submodule)
└── docs/
    ├── findings.md             # M5: real bugs found, with reproduction
    └── design.md               # M5: 1–2 page architecture writeup
```

---

## 4. Global engineering rules (apply to *every* milestone)

1. Every executable builds with `-Wall -Wextra -g` and **zero warnings**.
2. Every milestone adds tests; **`ctest` must pass** before a milestone is "done".
3. Provide `--help` on every tool, and a uniform diagnostic format:
   **`file:line:col: [rule] message`**.
4. No milestone may break a previous one. Keep `main` green (CI-style discipline).
5. Update `README.md` each milestone: how to build, how to run, what's implemented.
6. Prefer real inputs: `tests/fixtures/` proves correctness on tiny cases, but the tool
   must also run on the real code in `targets/`.
7. Commit the *validation evidence* (sample output, a saved crash, a findings entry) so
   a reviewer can see the milestone actually worked.

---

## 5. Milestones

Each milestone has four parts: **Concept** (what you're learning), **Build** (what to
make — guidance and key APIs, not the solution), **Commands**, and **✅ Validation**
(exact checks; all boxes must be tickable before moving on).

---

### Milestone 1 — Toolchain + your first AST walk

**Concept.** Before you can *judge* code you must *traverse* it. Clang parses a source
file into an **Abstract Syntax Tree** — every function, call, loop, and variable is a
typed node. LibTooling lets you run your own code over that tree. This milestone proves
your toolchain works and that you can read the AST.

**Build.**
- Scaffold the repo, `CMakeLists.txt`, `scripts/setup.sh`, and a README skeleton.
- A LibTooling tool (`src/static/main.cpp`) using `clang::tooling::CommonOptionsParser`
  that, given a source file, prints **every function definition** and **every function
  call** as `name + file:line`.
- Add two fixture files under `tests/fixtures/` (one with a couple of functions/calls).

> **Orientation (not the solution):** explore the AST first with the built-in tool —
> `clang-18 -Xclang -ast-dump -fsyntax-only tests/fixtures/sample.c` — to see the node
> names (`FunctionDecl`, `CallExpr`). Your tool will visit those same nodes via a
> `RecursiveASTVisitor` or AST matchers.

**Commands.**
```bash
cmake -S . -B build -G Ninja && cmake --build build
./build/sentinel-static tests/fixtures/sample.c -- -std=c11
```

**✅ Validation.**
- [ ] `cmake --build build` succeeds with **no warnings**.
- [ ] `sentinel-static tests/fixtures/sample.c -- -std=c11` lists **≥1 function** and
      **≥1 call**, each with the **correct line number** (cross-check by eye).
- [ ] Running on a file with a deliberate syntax error prints a clear diagnostic, not a
      crash.

---

### Milestone 2 — Static analyzer with AST matchers

**Concept.** **AST Matchers** are a declarative query language over the Clang AST:
you describe a shape ("a call to a function named `gets`") and Clang hands you every
node that fits. This is exactly how real linters and the Clang Static Analyzer find
patterns. You'll implement four rules, each catching a real bug class.

**Build.** Implement each rule as an AST-matcher bound to a callback that emits a
diagnostic in the standard format. Wire them behind flags (`--enable=R1,R3`; default
all on).

| Rule | What it flags | Why it's dangerous |
|------|---------------|--------------------|
| **R1** | calls to `gets`, `strcpy`, `strcat`, `sprintf` | classic buffer-overflow sinks |
| **R2** | floating-point equality (`==`/`!=` on floats) | float rounding → silent logic bugs |
| **R3** | assignment used as a condition (`if (x = y)`) | almost always a typo for `==` |
| **R4** | `malloc`/`new` whose result is not freed in the same function scope (best-effort, intra-procedural) | memory leak |

> **One worked example of the API shape (R1), so you learn the pattern — the other
> three are yours to design:** an unsafe-call matcher looks like
> `callExpr(callee(functionDecl(hasName("gets"))))`. Bind it with `.bind("call")`,
> register a `MatchCallback`, and in `run()` pull the node with
> `Result.Nodes.getNodeAs<CallExpr>("call")` to get its source location. R2–R4 use
> other matcher verbs (`binaryOperator`, `hasOperatorName`, `ifStmt`, `hasCondition`,
> …) — consult the **AST Matcher Reference**.

For each rule, add a **bad** fixture (the rule must fire) and a **clean** fixture (the
rule must stay silent), and a GoogleTest asserting both.

**Commands.**
```bash
cmake --build build
ctest --test-dir build --output-on-failure
./build/sentinel-static targets/<somefile.c> -- -std=c11
```

**✅ Validation.**
- [ ] `ctest` is **green**, and **every rule has both** a positive and a negative test.
- [ ] Each rule, run on its bad fixture, prints exactly one well-formed
      `file:line:col: [Rn] message`.
- [ ] Running on real code in `targets/` produces a **non-empty, correctly formatted**
      findings list.
- [ ] `--enable=R1` runs *only* R1 (flag wiring works).

---

### Milestone 3 — Dynamic analysis: an LLVM instrumentation pass

**Concept.** Sanitizers (Module 0) work by *instrumenting* your program — inserting
extra instructions that check for bad behavior at runtime. Now you build a tiny version
yourself. An **LLVM pass** runs over the program's **IR** (the compiler's intermediate
representation, between AST and machine code) and you *rewrite* it: before each risky
operation, insert a call to a runtime check.

```
  source.c ──clang──► LLVM IR ──[YOUR PASS rewrites it]──► instrumented IR ──► binary
                                       │                                          │
                          inserts "call __sentinel_check(ptr, idx, len)"   aborts at runtime
                          before each array/pointer access                 with a clear message
```

**Build.**
- A **New Pass Manager pass plugin** (`src/pass/BoundsPass.cpp`) that instruments **one**
  error class — choose **array/pointer bounds** *or* **signed integer overflow**.
- A runtime support function (`src/pass/runtime.c`) the inserted code calls, e.g.
  `void __sentinel_check(...)` that prints `runtime error: <kind> at <loc>` and aborts.
- Build the pass as a shared object loadable via
  `opt -load-pass-plugin=./BoundsPass.so -passes=sentinel-bounds` **and** via
  `clang -fpass-plugin=./BoundsPass.so`. Put the wrapper in `scripts/build_with_pass.sh`.
- `tests/pass_tests/`: a buggy program that must **abort with your message** once
  instrumented, and a clean program that must **exit 0**.

> **Orientation:** start from the official *"Writing an LLVM Pass (New Pass Manager)"*
> template and the `llvmGetPassPluginInfo` entry point. Iterate over instructions;
> for the bounds variant you instrument `GetElementPtr`/`Load`/`Store`; insert your
> check with `IRBuilder`. Inspect IR with `clang -O1 -S -emit-llvm foo.c -o foo.ll`.

**Commands.**
```bash
cmake --build build                                  # builds BoundsPass.so
./scripts/build_with_pass.sh tests/pass_tests/oob.c  /tmp/oob
./scripts/build_with_pass.sh tests/pass_tests/ok.c   /tmp/ok
/tmp/oob ; echo "exit=$?"     # expect: your "runtime error" message, non-zero exit
/tmp/ok  ; echo "exit=$?"     # expect: exit=0, no message
```

**✅ Validation.**
- [ ] The pass builds as a `.so` with **no warnings** and loads under both `opt` and
      `clang -fpass-plugin`.
- [ ] The planted buggy program, compiled **with** the pass, **aborts with your exact
      diagnostic**; compiled **without** the pass it does *not* (proving the pass is
      what catches it).
- [ ] The matching clean program exits **0** under the pass (no false positive).
- [ ] A `pass_tests` entry in `ctest` automates the above.

---

### Milestone 4 — Fuzzing: find a real crash

**Concept.** A **fuzzer** feeds a function a flood of mutated inputs, guided by code
coverage, hunting for inputs that crash it. Combined with **AddressSanitizer**, even a
silent memory corruption becomes a loud, located crash with a reproducer. This is how
a huge share of real-world CVEs are found.

```
  seed inputs ─► libFuzzer ─► mutate ─► call LLVMFuzzerTestOneInput(data,size)
                    ▲                              │
                    └──── coverage feedback ───────┘   ── crash? save the input, minimize it
```

**Build.**
- A libFuzzer harness (`src/fuzz/parse_fuzzer.cpp`) exposing
  `extern "C" int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size)` that drives
  a parser-like function — either a small bundled target with a planted bug, or a real
  function from `targets/`.
- Build it with `clang -g -fsanitize=fuzzer,address,undefined`.
- A `corpus/` seed directory and `scripts/fuzz.sh` that runs for a bounded time
  (e.g. `-max_total_time=60`).
- When it crashes: **minimize** the input (`-minimize_crash=1`) and write a short
  **root-cause** note (which line, which bug class, why).

**Commands.**
```bash
clang -g -fsanitize=fuzzer,address,undefined src/fuzz/parse_fuzzer.cpp -o build/fuzzer
./scripts/fuzz.sh build/fuzzer corpus/        # runs bounded; saves crash-* on failure
./build/fuzzer crash-<hash>                    # replays the saved crashing input
```

**✅ Validation.**
- [ ] `scripts/fuzz.sh` finds a crash on the seeded buggy target within the bounded run
      (e.g. ≤ 60 s) and **saves the reproducer file**.
- [ ] Re-running the binary on the saved reproducer **deterministically crashes** with
      an ASan report pointing at the faulty line.
- [ ] A minimized reproducer and a 3–5 line root-cause note are committed.

---

### Milestone 5 — Orchestrator CLI, a real run, and polish

**Concept.** A security tool is only useful if someone can point it at a real codebase
and get a report. You'll unify the three engines behind one CLI and prove the toolkit on
**real open-source code**, producing a findings report — the actual deliverable.

**Build.**
- `src/cli/sentinel.cpp`: takes a path, runs the static rules over all sources,
  optionally builds-with-pass and/or fuzzes designated entry points, and prints a
  **consolidated report** (human-readable text **and** a `--json` machine-readable
  option).
- Vendor a small real C project under `targets/`, run Sentinel on it, and record genuine
  findings in `docs/findings.md` with reproduction steps.
- Finalize `README.md` (architecture, usage, results) and write `docs/design.md`
  (1–2 pages: how the three engines fit, what you'd do next).

**Commands.**
```bash
./build/sentinel targets/<project>/             # consolidated text report
./build/sentinel targets/<project>/ --json > report.json
```

**✅ Validation.**
- [ ] `sentinel <target-dir>` runs **end-to-end** and emits a consolidated report; the
      `--json` output is valid JSON (pipe through `jq .`).
- [ ] `docs/findings.md` contains **≥1 genuine issue** found in real code, each with
      reproduction steps a reviewer can follow.
- [ ] A newcomer can clone, run `scripts/setup.sh`, build, and reproduce the report in
      **under 10 minutes** following only the README.

---

## 6. Definition of done (whole project)

- Clean build, **zero warnings**, `ctest` green on `main`.
- Static analyzer (**≥4 rules**), an **LLVM instrumentation pass**, and a working
  **fuzzer**, all integrated behind one CLI.
- Demonstrated on **real code** with a written findings report (`docs/findings.md`).
- README + `docs/design.md` let a newcomer build, run, and understand everything quickly.

---

## 7. Troubleshooting

| Symptom | Likely cause / fix |
|---------|--------------------|
| CMake: `Could not find LLVM` / `Clang` | `LLVM_DIR` / `Clang_DIR` not exported, or wrong version. Re-source `scripts/setup.sh`; confirm `ClangConfig.cmake` exists (section 2.2). |
| LibTooling tool prints `unknown argument` for your file | You forgot the `--` separator. Compiler flags go **after** `--`: `tool file.c -- -std=c11`. |
| AST matcher never fires | Your matcher shape doesn't match the real tree. Dump it: `clang-18 -Xclang -ast-dump -fsyntax-only file.c` and compare node names. |
| Pass plugin loads but does nothing | Pass name in `-passes=` must equal the name you registered in `llvmGetPassPluginInfo`. |
| Fuzzer: `undefined reference to LLVMFuzzerTestOneInput` | Missing `-fsanitize=fuzzer`; it provides `main`. Don't define your own `main` in the harness. |
| Sanitizer report but no line numbers | Build with `-g`, and set `ASAN_OPTIONS=symbolize=1` (and have `llvm-symbolizer` on `PATH`). |

---

## 8. References (consult, do not copy)

- Clang **LibTooling** and the **AST Matcher Reference** (official Clang docs).
- LLVM **"Writing an LLVM Pass" (New Pass Manager)** and the **Programmer's Manual**.
- **libFuzzer** documentation and the **AddressSanitizer / UBSan** docs.
- For the bug classes themselves: **CWE** entries for buffer overflow (CWE-787/125),
  use-after-free (CWE-416), and integer overflow (CWE-190).
