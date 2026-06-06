# How to work on this track (and how to submit)

This is a learning track. You work on **your own fork**, one milestone at a time,
on its own branch, and **submit each milestone by email**. This page is the single
source of truth for the workflow.

```
  ── The workflow, end to end ──

   1. FORK ──► 2. CLONE ──► 3. MODULE 0 ──► 4. PICK A PROJECT ──► 5. BRANCH PER MILESTONE
   (your copy   (your        (everyone        (Sentinel OR          (sentinel/milestone-1, …)
    on GitHub)   machine)     does this)       Lumen)                     │
                                                                          ▼
                                          8. SUBMIT BY EMAIL ◄── 7. PUSH ◄── 6. VALIDATE
                                          (repo link + branch)   (to your    (the SPEC's ✅
                                                                  fork)        checklist passes)
```

---

## 1. Fork the repository

Click **Fork** on `https://github.com/ykro/ai-compilers-systems-prep` to create your
own copy under your GitHub account. **All your work happens in your fork** — you never
push to the original.

## 2. Clone your fork and set up

```bash
git clone https://github.com/<your-username>/ai-compilers-systems-prep.git
cd ai-compilers-systems-prep
```

## 3. Do Module 0 first

Everyone completes [`module-0-cpp-onramp/`](module-0-cpp-onramp/) before starting a
project. Work through the lessons and the capstone bug hunt. You can do this directly
on a `module-0` branch in your fork.

## 4. Pick your project

- **Project 1 — Sentinel** ([spec](project-1-sentinel/SPEC.md)) — static/dynamic analysis
  + fuzzing. Needs a Linux + LLVM 18 toolchain.
- **Project 2 — Lumen** ([spec](project-2-lumen/SPEC.md)) — CUDA GPU path tracer. Needs a
  free Google Colab T4 GPU (no local GPU).

The two projects are independent — one person takes one.

## 5. One branch per milestone

Create a **new branch for each milestone**, off your fork's `main`. Use this naming:

| Work | Branch name |
|------|-------------|
| Module 0 | `module-0` |
| Sentinel milestone *N* | `sentinel/milestone-N` |
| Lumen milestone *N* | `lumen/milestone-N` |

```bash
git checkout main
git checkout -b sentinel/milestone-1     # example
```

Do not start a new milestone branch until the previous milestone's **Validation**
checklist (in the SPEC) fully passes.

## 6. Validate before you submit

Each milestone in the SPEC ends with a **✅ Validation** checklist. **Every box must be
tickable** — that is the definition of "done" for the milestone. Commit the evidence the
checklist asks for (sample output, a rendered image, a saved crash, a findings entry).

## 7. Commit and push to your fork

```bash
git add -A
git commit -m "Sentinel milestone 1: AST walk over a C file"
git push -u origin sentinel/milestone-1
```

## 8. Submit the milestone by email

Send an email for each completed milestone to:

> **adriancatalan@galileo.edu**

**Subject:**
```
[AI-Compilers-Systems] <Your Full Name> — <Sentinel|Lumen> Milestone <N>
```

**Body — include exactly these two things:**

1. **Link to your fork:** `https://github.com/<your-username>/ai-compilers-systems-prep`
2. **The branch with the milestone:** e.g. `sentinel/milestone-1`

Optionally add a one-line note (e.g. "all validation checks pass; rendered image in
`gallery/m1.png`").

> Make sure the branch is **pushed to your public fork** before sending, so the link and
> branch are visible.

---

## Ground rules (from the SPECs — they apply to every milestone)

- Build with `-Wall -Wextra` and **zero warnings**.
- Keep your `main` always building; do milestone work on its own branch.
- Tests/validation pass before a milestone counts as done.
- Update the project `README.md` as you go (how to build, run, what's implemented).
