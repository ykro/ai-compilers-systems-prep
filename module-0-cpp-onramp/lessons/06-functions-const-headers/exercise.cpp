// Lesson 06 exercise - finish clampi (in mathutils.cpp) and the sum below.
// Build (two files!):
//   g++ -std=c++17 -Wall -Wextra -g -I../../include exercise.cpp mathutils.cpp -o ex && ./ex
#include "check.h"
#include <vector>
#include "mathutils.h"

// TODO: implement using the const-ref `sum` from mathutils.h (do NOT copy xs).
int sum_clamped(const std::vector<int>& xs, int lo, int hi) {
    // Hint: clamp each element with clampi, then add. You may use a local vector
    // or just accumulate directly. Return the total.
    (void)xs; (void)lo; (void)hi;
    return 0;   // <-- replace
}

int main() {
    std::printf("Lesson 06 exercise\n");
    CHECK(clampi(5, 0, 10) == 5);
    CHECK(clampi(-3, 0, 10) == 0);
    CHECK(clampi(99, 0, 10) == 10);

    std::vector<int> v = {-5, 5, 50};
    CHECK(sum(v) == 50);                 // -5 + 5 + 50
    CHECK(sum_clamped(v, 0, 10) == 15);  // 0 + 5 + 10
    return SUMMARY();
}
