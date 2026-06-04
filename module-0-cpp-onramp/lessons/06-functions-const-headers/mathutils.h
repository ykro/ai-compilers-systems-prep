#ifndef MATHUTILS_H
#define MATHUTILS_H

#include <vector>

// Declarations only (no bodies). Both example.cpp and exercise.cpp include this.

// Clamp v into the inclusive range [lo, hi].
int clampi(int v, int lo, int hi);

// Sum a vector WITHOUT copying it (read-only by const reference).
int sum(const std::vector<int>& xs);

#endif // MATHUTILS_H
