// check.h — a tiny test helper for the C/C++ onramp exercises.
//
// Each exercise.cpp ships with a set of CHECK(...) assertions in main().
// The file compiles as-is, but the checks FAIL until you implement the TODOs.
// When every check passes, main() returns 0 (a green build/run).
//
// This is deliberately minimal — no dependencies, just <cstdio>. It exists so
// you can *see* your progress without learning a full test framework yet
// (you meet GoogleTest later, in Project 1 — Sentinel).
#pragma once

#include <cstdio>

// One global failure counter. `inline` (C++17) lets this header-only variable
// be included from multiple translation units without a linker error.
inline int g_checks_failed = 0;

// CHECK(cond): pass if `cond` is true, otherwise record a failure with location.
#define CHECK(cond)                                                            \
    do {                                                                       \
        if (cond) {                                                            \
            std::printf("  [PASS] %s\n", #cond);                               \
        } else {                                                               \
            std::printf("  [FAIL] %s   (%s:%d)\n", #cond, __FILE__, __LINE__); \
            ++g_checks_failed;                                                 \
        }                                                                      \
    } while (0)

// CHECK_EQ(a, b): convenience wrapper for equality checks.
#define CHECK_EQ(a, b) CHECK((a) == (b))

// Call once at the end of main(): prints a summary and returns a process
// exit code (0 = all passed, 1 = something failed). Use it as:
//     return check_report();
inline int check_report() {
    if (g_checks_failed == 0) {
        std::printf("All checks passed. \xE2\x9C\x93\n");
        return 0;
    }
    std::printf("%d check(s) failed. Keep going.\n", g_checks_failed);
    return 1;
}
