# Project 2 — "Lumen": GPU Path Tracer (CUDA, runs on Colab)
### Build brief for Claude Code

> **How to use this file:** paste it into Claude Code as the project spec. Work **one milestone at a time**, on its own git branch, PR per milestone. **Important:** Claude Code likely has no NVIDIA GPU, so it cannot run the `.cu` programs itself. Claude Code must therefore (a) write code that *compiles* and is reviewed locally, and (b) provide a Colab notebook + commands so the **user runs and validates on a free Colab T4 GPU**. Every milestone's acceptance is checked by the user on Colab.

---

## 1. Goal

Build a **Monte-Carlo path tracer that runs on the GPU with CUDA**, then **profile and optimize it** and measure the speedups. The payoff is visual (rendered PNG images) and quantitative (before/after timing tables). This mirrors the HTEC internship's *GPU software* and *HPC* work.

The structural reference is the *Ray Tracing in One Weekend* book series (CPU) and its known CUDA port — Claude Code should follow that structure conceptually, not copy text.

---

## 2. Environment & stack

- **GPU:** NVIDIA **T4** on Google Colab free tier (16 GB, CUDA 12.x, `nvcc` preinstalled). No local GPU required.
- **Language:** CUDA C++ (C++17 host code).
- **Image output:** `stb_image_write.h` (single public-domain header; vendor it into `third_party/`). Write PPM as a fallback if needed.
- **Timing:** CUDA events (`cudaEventRecord`/`cudaEventElapsedTime`).
- **Build:** a simple `Makefile` and an optional `CMakeLists.txt` with CUDA language enabled. Each milestone should compile with:
  ```bash
  nvcc -std=c++17 -O3 -arch=sm_75 src/main.cu -o build/lumen   # sm_75 = T4
  ```
- **Colab note:** CPU runtime is wrong; the user must select **Runtime → Change runtime type → T4 GPU**. Profiling with Nsight Compute counters is usually restricted on Colab, so **all optimization is measured with CUDA event timing**, not `ncu`.

---

## 3. Target repository layout

```
lumen/
├── README.md
├── Makefile
├── CMakeLists.txt
├── third_party/
│   └── stb_image_write.h
├── src/
│   ├── vec3.cuh                # vector math (host+device)
│   ├── ray.cuh
│   ├── camera.cuh
│   ├── hittable.cuh            # spheres, world
│   ├── material.cuh            # diffuse / metal / dielectric
│   ├── render.cu               # the GPU kernels
│   └── main.cu                 # parse args, launch, write PNG, print timings
├── scenes/                     # scene definitions or generators
├── benchmarks/
│   └── results.md              # milestone 4: before/after timing tables
├── gallery/                    # rendered output images checked in
└── notebooks/
    └── lumen_colab.ipynb       # build+run+display on Colab T4
```

Claude Code must keep `notebooks/lumen_colab.ipynb` in sync: cells that `git clone`, compile with `nvcc`, run, and display the output PNG with `IPython.display.Image`.

---

## 4. Global engineering rules

1. Wrap every CUDA API call in a `CUDA_CHECK(...)` macro that checks the return status and reports `file:line` on error.
2. After every kernel launch, check `cudaGetLastError()` and `cudaDeviceSynchronize()`.
3. Time every render with CUDA events and print `ms` + computed Mrays/s.
4. Each milestone produces a committed image in `gallery/` so progress is visually verifiable.
5. Keep host/device code buildable for `sm_75` (T4). Note in README how to change `-arch` for other GPUs.
6. Deterministic seeds for reproducible images (seed `curand` per pixel from pixel index + frame).

---

## 5. Milestones

### Milestone 1 — GPU fundamentals + first image
**Objective:** prove the toolchain and the GPU execution model with an embarrassingly-parallel image.
**Tasks:**
- Set up repo, `Makefile`, `CUDA_CHECK`, PNG writer via `stb_image_write`.
- Write a kernel that computes a **Mandelbrot set** (or a UV gradient) — one thread per pixel — and writes an RGB buffer.
- Copy device→host, save PNG. Print kernel time via CUDA events. Add a trivial CPU reference and print the CPU-vs-GPU speedup.
- Build the Colab notebook.
**Deliverable:** `lumen` renders a Mandelbrot PNG; notebook shows it on Colab.
**Acceptance criteria (user runs on Colab T4):**
- `nvcc` build succeeds; running produces a correct image in `gallery/`; timing table shows GPU faster than the CPU reference.

### Milestone 2 — Ray casting + sphere intersection
**Objective:** the core of a ray tracer on the GPU.
**Tasks:**
- Implement `vec3`, `ray`, `camera` (host+device headers).
- A kernel that, per pixel, generates a camera ray, intersects a small list of spheres, and shades by surface normal (the classic normal-as-color image).
**Deliverable:** rendered image of several shaded spheres + ground sphere.
**Acceptance criteria:** image matches the expected normal-shaded spheres; no CUDA errors; timing printed.

### Milestone 3 — Path tracing: bounces, materials, anti-aliasing
**Objective:** physically-based rendering.
**Tasks:**
- Per-pixel `curand` state; multiple samples per pixel (AA) accumulated.
- Recursive/iterative path tracing with a bounce limit.
- Materials: **diffuse (Lambertian), metal (reflection + fuzz), dielectric (refraction)**.
- Gamma-correct the output.
**Deliverable:** a "pretty" multi-material scene (the showcase image).
**Acceptance criteria:** image shows correct diffuse/metal/glass behavior; increasing samples reduces noise; runs within a reasonable time on T4 (document it).

### Milestone 4 — Profiling & optimization (the HPC core)
**Objective:** make it meaningfully faster and explain why.
**Tasks (apply, measure each with CUDA events, record in `benchmarks/results.md`):**
- Tune block/grid dimensions and report occupancy reasoning.
- Improve memory access patterns (coalescing); move read-only scene data to `__constant__` or texture/`__ldg`.
- Reduce warp divergence in the material/branch logic.
- Persistent `curand` state vs. re-init; minimize register pressure where it helps.
**Deliverable:** an **optimized renderer** + a before/after table (ms, Mrays/s, % gain per change) + a written explanation of each optimization.
**Acceptance criteria:** end-to-end render time on the same scene/T4 improves measurably vs. Milestone 3 (target ≥ 3×), with each gain attributed to a specific change.

### Milestone 5 — AI-compiler bridge + final demo
**Objective:** connect the project to AI/HPC kernels and ship.
**Tasks:**
- Add a separate program: a **tiled matrix-multiply (GEMM) kernel** using shared memory, benchmarked against a naive version (the canonical AI-kernel optimization). Optionally run a tiny neural-net forward pass on GPU.
- Build a gallery of images, finalize `benchmarks/results.md`, polish README (build, run, results, hardware notes), and a short writeup.
**Deliverable:** final repo, image gallery, GEMM benchmark, performance report.
**Acceptance criteria:** tiled GEMM beats naive GEMM with a documented speedup; the Colab notebook reproduces the showcase render and the benchmarks from a clean clone.

---

## 6. Definition of done (whole project)
- Path tracer renders correct diffuse/metal/dielectric scenes on a Colab T4 from a clean clone.
- Optimization milestone shows a measured, explained speedup (CUDA-event timing).
- GEMM tiling demo ties the work to AI/HPC kernels.
- README + notebook let a newcomer reproduce everything on free Colab with no local GPU.

## 7. References (consult, do not copy)
- *Ray Tracing in One Weekend* series (structure/algorithms).
- NVIDIA *"Accelerated Ray Tracing in One Weekend in CUDA"* (the GPU port approach).
- NVIDIA *CUDA C++ Programming Guide* and *"An Even Easier Introduction to CUDA"*.
- *Programming Massively Parallel Processors* (PMPP) for the optimization and GEMM tiling.
