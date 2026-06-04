#include "mathutils.h"

// TODO (Lesson 06): implement clampi so that:
//   clampi(5, 0, 10) == 5 ; clampi(-3, 0, 10) == 0 ; clampi(99, 0, 10) == 10
int clampi(int v, int lo, int hi) {
    (void)lo; (void)hi;
    return v;   // <-- replace with a correct clamp
}

int sum(const std::vector<int>& xs) {
    int total = 0;
    for (int x : xs) total += x;   // xs is read-only here (const&)
    return total;
}
