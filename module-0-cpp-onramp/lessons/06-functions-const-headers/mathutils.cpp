// mathutils.cpp — DEFINITIONS (the implementation of mathutils.h).
//
// This file is compiled on its own into mathutils.o, then linked with whatever
// program uses it (example.cpp or exercise.cpp). Change only this file and the
// callers don't need to be recompiled from scratch — that's the payoff of
// separating interface (.h) from implementation (.cpp).
#include "mathutils.h"

namespace mathutils {

int square(int n) {
    return n * n;
}

long sum(const std::vector<int>& v) {
    long total = 0;
    for (int x : v) total += x;       // we only READ v — that's why it's const&
    return total;
}

int clamp(int x, int lo, int hi) {
    if (x < lo) return lo;
    if (x > hi) return hi;
    return x;
}

}  // namespace mathutils
