# AI, Compilers & System Software — Internship Prep Track

A self-contained learning track for interns heading into **systems software**:
compilers, debuggers, GPU/HPC kernels, and the tools that find bugs and security
vulnerabilities in low-level code.

It has three parts: one shared onramp everyone does first, then **two
independent projects** — designed so **two people can each take one** and work
it end-to-end.

## Structure

| Part | Folder | What it is | Owner |
|------|--------|------------|-------|
| **Module 0** | [`module-0-cpp-onramp/`](module-0-cpp-onramp/) | A complete, runnable C/C++ onramp (6 lessons + capstone). **Everyone does this first.** | Shared |
| **Project 1** | [`project-1-sentinel/`](project-1-sentinel/SPEC.md) | **Sentinel** — a C/C++ bug & vulnerability analysis toolkit (Clang static analysis + an LLVM instrumentation pass + libFuzzer). | Person A |
| **Project 2** | [`project-2-lumen/`](project-2-lumen/SPEC.md) | **Lumen** — a CUDA GPU path tracer, profiled and optimized, runnable on a free Colab T4 GPU. | Person B |

## How to use this repo

1. **Start with Module 0.** It's about one week of work and makes pointers,
   memory, the compile/link model, and sanitizers concrete. Build instructions
   are in [`module-0-cpp-onramp/README.md`](module-0-cpp-onramp/README.md)
   (local CMake, or Google Colab with no install).

2. **Then split into the two projects.** Each project folder contains a
   `SPEC.md` — a detailed build brief meant to be executed **one milestone at a
   time**, on its own git branch, with a PR per milestone, only advancing once
   the previous milestone's acceptance criteria pass.

   - **Sentinel** (Project 1) leans on the static/dynamic-analysis and security
     side. Needs an LLVM 18 / Clang dev toolchain on Linux (or WSL2).
   - **Lumen** (Project 2) leans on GPU and HPC. Needs no local GPU — it builds
     and runs on a free Google Colab **T4** GPU; the spec includes a Colab
     notebook in its layout.

## Status

- **Module 0 — onramp:** complete and runnable (lessons, examples, exercises,
  capstone bug hunt).
- **Project 1 (Sentinel) & Project 2 (Lumen):** these are **specifications**,
  not yet implemented. Each `SPEC.md` is the source of truth for what the
  project must do; the implementation is the intern's work.

## The throughline

Module 0 makes you *feel* memory bugs by hand. **Sentinel** builds the tools
that find them automatically. **Lumen** takes the same low-level control and
points it at raw throughput on the GPU. Together they cover the internship's
domain: *"AI, Compilers & System Software."*
