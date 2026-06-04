// check.h - a tiny, dependency-free test helper for the onramp exercises.
// Coming from JUnit (Java) or pytest (Python)? This is a 20-line stand-in:
// CHECK(cond) records a pass/fail; SUMMARY() prints the tally and sets the
// process exit code (0 = all passed). No framework to install.
#ifndef ONRAMP_CHECK_H
#define ONRAMP_CHECK_H

#include <cstdio>

namespace onramp {
inline int g_passed = 0;
inline int g_failed = 0;
}

#define CHECK(cond)                                                         \
    do {                                                                    \
        if (cond) {                                                         \
            onramp::g_passed++;                                             \
            std::printf("  [PASS] %s\n", #cond);                            \
        } else {                                                            \
            onramp::g_failed++;                                             \
            std::printf("  [FAIL] %s   (%s:%d)\n", #cond, __FILE__, __LINE__);\
        }                                                                   \
    } while (0)

#define SUMMARY()                                                           \
    (std::printf("\n%d passed, %d failed\n", onramp::g_passed, onramp::g_failed), \
     onramp::g_failed == 0 ? 0 : 1)

#endif // ONRAMP_CHECK_H
