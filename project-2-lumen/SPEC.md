# Project 2 — "Lumen": GPU Path Tracer (CUDA, runs on Colab)
### Project specification (a guided learning project)

> **How to use this file:** this document is the complete source of truth for *what to
> build and how to know it works*. It is written so you can do the whole project from it
> alone — the CUDA source is yours to write; nothing is implemented for you.
>
> Work **one milestone at a time**, each on its own git branch in your fork. **Do not
> start a milestone until the previous one's *Validation* checklist passes on a real
> GPU.** The fork/branch/submit workflow is in [`CONTRIBUTING.md`](../CONTRIBUTING.md) —
> each milestone is submitted by email (repo link + branch).
>
> **Important about hardware:** your development machine likely has **no NVIDIA GPU**, so
> you cannot *run* the `.cu` programs locally. The workflow is therefore: (a) write code
> that **compiles** and review it locally, and (b) keep a **Colab notebook** in sync so
> every milestone is **run and validated on a free Colab T4 GPU**. Every ✅ check below is
> performed on Colab.

---

## 1. What you are building (and why)

A **Monte-Carlo path tracer that runs on the GPU with CUDA** — then you **profile and
optimize it** and measure the speedups. The payoff is both **visual** (rendered PNG
images you can look at) and **quantitative** (before/after timing tables). This mirrors
the internship's **GPU software** and **HPC** work.

A path tracer simulates light: for each pixel you shoot a ray into the scene, bounce it
off surfaces, and average many random samples to get a color. It is "embarrassingly
parallel" — every pixel is independent — which makes it a perfect vehicle for learning
the GPU.

The structural reference is the *Ray Tracing in One Weekend* book series (CPU) and its
known CUDA port — follow that structure conceptually, not by copying text.

### The GPU mental model

A GPU runs **thousands of threads at once**. CUDA organizes them into a **grid of
blocks**, each block a group of **threads**. You map **one thread to one pixel**:

```
  ── How pixels map onto the GPU ──

   final image (W × H)            CUDA launch: grid of blocks            each GPU thread does:
   ┌───────────────────┐          ┌────┬────┬────┐                       ┌────────────────────┐
   │███████████████████│          │ B  │ B  │ B  │   each block =        │ for ITS one pixel: │
   │███████████████████│  maps to │ B  │ B  │ B  │   16×16 threads       │  • make camera ray │
   │███████████████████│ ───────► │ B  │ B  │ B  │   1 thread = 1 pixel  │  • intersect world │
   │███████████████████│          │ B  │ B  │ B  │                       │  • shade / bounce  │
   └───────────────────┘          └────┴────┴────┘                       │  • write its color │
                                                                         └────────────────────┘
```

### The render pipeline (one frame)

```
   HOST (CPU)                                     DEVICE (GPU)
   ──────────                                     ────────────
   build scene (spheres, materials)
        │   cudaMalloc + cudaMemcpy
        ▼
   launch render<<<grid, block>>> ─────────────►  every pixel, in parallel:
        │                                            init curand(seed = pixel index)
        │   wrap with cudaEvent start/stop            repeat N samples:
        │                                               ray = camera.get_ray(u, v)
        │                                               color += trace(ray)   ◄──┐ up to
        │                                            framebuffer[pixel] = color   │ MAX_DEPTH
        ▼   cudaMemcpy back                                                       │ bounces
   gamma-correct + write PNG  ◄─────────────────  framebuffer ───────────────────┘
        │
        ▼
   print ms + Mrays/s   (computed from the cudaEvent timing)
```

Keep these two diagrams in mind for every milestone — each one fills in a piece of this
same pipeline.

---

## 2. Prerequisites & environment

You should be comfortable with Module 0 (pointers, value vs. reference, the heap). CUDA
C++ extends C++ with a few keywords (`__global__`, `__device__`, `<<<...>>>`); you'll
learn those here.

| Component | Requirement |
|-----------|-------------|
| GPU       | NVIDIA **T4** on Google Colab free tier (16 GB, CUDA 12.x, `nvcc` preinstalled). No local GPU required. |
| Language  | CUDA C++ (C++17 host code) |
| Image out | `stb_image_write.h` (single public-domain header; vendor into `third_party/`). PPM as a fallback. |
| Timing    | CUDA events (`cudaEventRecord` / `cudaEventElapsedTime`) |
| Build     | a simple `Makefile` (and an optional `CMakeLists.txt` with CUDA enabled) |

Each milestone must compile with:

```bash
nvcc -std=c++17 -O3 -arch=sm_75 src/main.cu -o build/lumen     # sm_75 = the T4's architecture
```

> **Note on what to compile:** the line above assumes `main.cu` `#include`s the kernels
> (e.g. `render.cu`/`.cuh` are header-style and pulled in by `main.cu`). If you keep
> `render.cu` as a **separate translation unit**, list every `.cu` on the command:
> `nvcc ... src/main.cu src/render.cu -o build/lumen`. Either design is fine — just keep
> the `Makefile` and the Colab notebook in agreement with whichever you choose.

> **Colab gotcha — read this once:** the default Colab runtime is **CPU**, where `nvcc`
> exists but no GPU does, so your program will fail at the first CUDA call. You **must**
> select **Runtime → Change runtime type → T4 GPU** before running. Also: Nsight Compute
> hardware counters (`ncu`) are usually **restricted** on Colab, so in this project **all
> optimization is measured with CUDA-event timing**, not `ncu`.

### 2.1 ✅ Validate your Colab environment *before Milestone 1*

In a notebook cell, confirm you actually have the GPU:

```bash
!nvidia-smi            # must show a "Tesla T4" — if this errors, you're on a CPU runtime
!nvcc --version        # must print CUDA 12.x
```

---

## 3. Repository layout

Produce roughly this structure (you create every file):

```
lumen/
├── README.md                   # build, run, results, hardware notes (update each milestone)
├── Makefile
├── CMakeLists.txt              # optional, CUDA language enabled
├── third_party/
│   └── stb_image_write.h       # vendored single-header PNG writer
├── src/
│   ├── vec3.cuh                # vector math (host + device)
│   ├── ray.cuh
│   ├── camera.cuh
│   ├── hittable.cuh            # spheres, the world
│   ├── material.cuh            # diffuse / metal / dielectric
│   ├── render.cu               # the GPU kernels
│   └── main.cu                 # parse args, launch, write PNG, print timings
├── scenes/                     # scene definitions or generators
├── benchmarks/
│   └── results.md              # M4: before/after timing tables
├── gallery/                    # rendered output images, committed
└── notebooks/
    └── lumen_colab.ipynb       # build + run + display on Colab T4
```

Keep `notebooks/lumen_colab.ipynb` in sync at all times: cells that `git clone` the
repo, compile with `nvcc`, run, and display the output PNG with `IPython.display.Image`.
The notebook *is* how a reviewer validates you.

---

## 4. Global engineering rules (apply to *every* milestone)

1. Wrap **every** CUDA API call in a `CUDA_CHECK(...)` macro that checks the returned
   `cudaError_t` and reports `file:line` on failure. (Silent CUDA errors are the #1 time
   sink in this project.)
2. After **every** kernel launch, check `cudaGetLastError()` then `cudaDeviceSynchronize()`.
3. Time every render with CUDA events and print **`ms`** and computed **Mrays/s**.
4. Each milestone commits an image in `gallery/` so progress is **visually** verifiable.
5. Keep host/device code buildable for `sm_75` (T4); note in the README how to change
   `-arch` for other GPUs.
6. Use **deterministic seeds** (seed `curand` per pixel from pixel index + frame number)
   so images are reproducible and diffable.

> **The one macro you write first** — everything else depends on it:
> ```cpp
> #define CUDA_CHECK(x) do { cudaError_t e=(x); if(e!=cudaSuccess){ \
>   fprintf(stderr,"CUDA error %s at %s:%d\n",cudaGetErrorString(e),__FILE__,__LINE__); \
>   exit(1);} } while(0)
> ```

---

## 5. Milestones

Each milestone has four parts: **Concept**, **Build** (what to make — guidance, not the
solution), **Commands**, and **✅ Validation** (run on Colab T4; all boxes tickable
before moving on).

---

### Milestone 1 — GPU fundamentals + your first image

**Concept.** Learn the GPU execution model with an embarrassingly-parallel task before
adding any ray-tracing math. One thread computes one pixel; you copy the result back and
save it. You also establish the timing harness you'll use for the rest of the project.

**Build.**
- Set up the repo, `Makefile`, the `CUDA_CHECK` macro, and a PNG writer via
  `stb_image_write`.
- A `__global__` kernel that computes a **Mandelbrot set** (or a UV gradient) — one
  thread per pixel — writing an RGB byte buffer in device memory.
- Copy device→host, save the PNG. Time the kernel with CUDA events. Add a trivial **CPU
  reference** version and print the **GPU-vs-CPU speedup**.
- Build the Colab notebook (clone → `nvcc` → run → display image).

> **Orientation (not the solution):** the per-pixel index pattern is
> `int x = blockIdx.x*blockDim.x + threadIdx.x;` (and similarly for `y`); guard with
> `if (x>=W || y>=H) return;`. Launch with a 2-D grid sized to cover `W×H`.

**Commands.**
```bash
nvcc -std=c++17 -O3 -arch=sm_75 src/main.cu -o build/lumen
./build/lumen --out gallery/m1.png
```

**✅ Validation (Colab T4).**
- [ ] `nvcc` build succeeds; the program runs with **no CUDA errors** (your `CUDA_CHECK`
      stays quiet).
- [ ] `gallery/m1.png` is a correct Mandelbrot/gradient image, displayed in the notebook.
- [ ] The timing table prints `ms` + `Mrays/s` and shows the **GPU faster than the CPU
      reference** on a non-trivial resolution.

---

### Milestone 2 — Ray casting + sphere intersection

**Concept.** The geometric core of a ray tracer: turn each pixel into a **ray** from a
**camera**, find the nearest **sphere** it hits, and color it by the surface **normal**
(the classic pastel "normal map" image). No lighting yet — just geometry.

**Build.**
- Implement `vec3` (host + device math), `ray`, and a `camera` that maps pixel
  coordinates `(u, v)` to a world-space ray.
- A kernel that, per pixel, generates the camera ray, intersects a **small list of
  spheres**, and shades the closest hit by its normal. Include a large "ground" sphere.

> **Orientation:** ray–sphere intersection is the quadratic
> `t² (d·d) + 2t (d·(o−c)) + (o−c)·(o−c) − r² = 0`; the smallest positive root in your
> `[t_min, t_max]` window is the nearest hit. Map a unit normal `n` to color as
> `0.5*(n+1)`.

**Commands.**
```bash
nvcc -std=c++17 -O3 -arch=sm_75 src/main.cu -o build/lumen
./build/lumen --scene spheres --out gallery/m2.png
```

**✅ Validation (Colab T4).**
- [ ] The image shows several **normal-shaded spheres** sitting on a ground sphere, with
      smooth color gradients across each surface.
- [ ] No CUDA errors; kernel timing is printed.
- [ ] Moving the camera (a CLI arg) visibly changes the viewpoint (proves the camera
      math is real, not hard-coded).

---

### Milestone 3 — Path tracing: bounces, materials, anti-aliasing

**Concept.** Make it physically based. Instead of one ray per pixel, shoot **many random
samples** and **bounce** rays off surfaces, accumulating light. Different **materials**
scatter light differently. This is where it starts to look real — and where each pixel
needs its **own random-number stream**.

**Build.**
- Per-pixel `curand` state; **N samples per pixel** (anti-aliasing) accumulated and
  averaged.
- Iterative path tracing with a **bounce/depth limit** (`MAX_DEPTH`).
- Three materials: **diffuse (Lambertian)**, **metal (reflection + fuzz)**, **dielectric
  (refraction, e.g. glass)**.
- **Gamma-correct** the final color (`sqrt`) before writing.

> **Orientation:** prefer an **iterative** bounce loop over recursion — deep recursion
> blows the GPU stack. Carry an `attenuation` color and multiply it each bounce; stop at
> `MAX_DEPTH` or when a ray escapes to the background.

**Commands.**
```bash
./build/lumen --scene showcase --spp 100 --depth 50 --out gallery/m3.png
```

**✅ Validation (Colab T4).**
- [ ] The showcase image shows **correct diffuse, metal, and glass** behavior (matte
      surfaces, mirror-like reflections, refraction through the dielectric).
- [ ] **Increasing `--spp` visibly reduces noise** (compare 10 vs 100 samples — commit
      both to `gallery/`).
- [ ] Render time on the T4 is documented (it's the **baseline** for Milestone 4).

---

### Milestone 4 — Profiling & optimization (the HPC core)

**Concept.** The heart of the project. A correct kernel is the start; making it **fast**
and **explaining why** is the HPC skill. You'll apply optimizations one at a time and
**measure each** with CUDA events, attributing every speedup to a specific change.

```
   baseline (M3)  ──►  +block/grid tuning  ──►  +coalesced memory  ──►  +less divergence  ──►  optimized
        │                    │                        │                       │                    │
        └─ each step: re-time the SAME scene on the SAME T4, record ms / Mrays/s / % gain ─────────┘
                              (write every row into benchmarks/results.md)
```

**Build (apply, measure each, record in `benchmarks/results.md`).**
- Tune **block/grid dimensions**; report occupancy reasoning.
- Improve **memory access patterns** (coalescing); move read-only scene data to
  `__constant__` memory or use texture/`__ldg`.
- Reduce **warp divergence** in the material/branch logic.
- **Register pressure / curand state**: persistent vs. re-init; minimize registers where
  it helps occupancy.

**Commands.**
```bash
# Re-run the identical scene after each change and append a row to results.md:
./build/lumen --scene showcase --spp 100 --depth 50 --out /tmp/opt.png
```

**✅ Validation (Colab T4).**
- [ ] `benchmarks/results.md` has a **before/after table**: ms, Mrays/s, and **% gain per
      change**, each row tied to one named optimization.
- [ ] End-to-end render time on the **same scene/T4** improves **measurably vs. Milestone
      3 (target ≥ 3×)**.
- [ ] The optimized image is **visually identical** to the M3 baseline (optimizing must
      not change the picture — commit both and diff).

---

### Milestone 5 — GEMM kernel bridge + final demo

**Concept.** Connect the path tracer's GPU skills to the **HPC/AI-kernel** world. A
**tiled matrix-multiply (GEMM)** using shared memory is *the* canonical GPU optimization —
the same coalescing/tiling ideas, on the operation that dominates scientific and ML
compute. You'll show tiling beats the naive version, then package everything.

**Build.**
- A **separate** program: a **tiled GEMM kernel** using shared-memory tiles, benchmarked
  against a **naive** (global-memory) GEMM. (Optional stretch: a tiny neural-net forward
  pass on the GPU.)
- Build an image **gallery**, finalize `benchmarks/results.md`, polish the README (build,
  run, results, hardware notes), and write a short performance writeup.

> **Orientation:** the tiled kernel loads a `TILE×TILE` block of each input into
> `__shared__` memory, `__syncthreads()`, then accumulates the partial product — so each
> global element is read once per tile instead of once per output element.

**Commands.**
```bash
nvcc -std=c++17 -O3 -arch=sm_75 src/gemm.cu -o build/gemm
./build/gemm --n 1024     # prints naive vs tiled GFLOP/s
```

**✅ Validation (Colab T4).**
- [ ] **Tiled GEMM beats naive GEMM** with a documented speedup, and both produce the
      **same result matrix** (correctness check against a CPU reference on a small size).
- [ ] The Colab notebook reproduces the **showcase render** *and* the **benchmarks** from
      a **clean clone** end-to-end.
- [ ] `gallery/` tells the story: Mandelbrot → normals → showcase → optimized.

---

## 6. Definition of done (whole project)

- Path tracer renders correct **diffuse / metal / dielectric** scenes on a Colab T4 from
  a clean clone.
- The optimization milestone shows a **measured, explained** speedup (CUDA-event timing).
- The **tiled GEMM** demo ties the work to HPC/AI kernels and beats the naive version.
- README + notebook let a newcomer reproduce **everything** on free Colab with no local GPU.

---

## 7. Troubleshooting

| Symptom | Likely cause / fix |
|---------|--------------------|
| First CUDA call fails / `no CUDA-capable device` | You're on a **CPU** Colab runtime. Runtime → Change runtime type → **T4 GPU**. |
| Image is all black / all one color | Kernel never wrote, or you forgot `cudaDeviceSynchronize()` before copying back. Check `cudaGetLastError()` right after the launch. |
| `nvcc` error: unsupported `-arch` | The T4 is `sm_75`. Other GPUs differ; make `-arch` a Makefile variable. |
| Garbage/striped image | Non-coalesced or out-of-bounds writes. Re-check the `if (x>=W||y>=H) return;` guard and the row-major index `y*W + x`. |
| Random "fireflies" that never go away | Each pixel needs its **own** `curand` state seeded by pixel index; a shared/un-seeded RNG correlates samples. |
| Stack overflow / kernel times out | Recursive bounces too deep — convert to an **iterative** loop with `MAX_DEPTH`. |

---

## 8. References (consult, do not copy)

- *Ray Tracing in One Weekend* series — structure and algorithms.
- NVIDIA *"Accelerated Ray Tracing in One Weekend in CUDA"* — the GPU port approach.
- NVIDIA *CUDA C++ Programming Guide* and *"An Even Easier Introduction to CUDA."*
- *Programming Massively Parallel Processors* (PMPP) — the optimization chapters and GEMM
  tiling.
