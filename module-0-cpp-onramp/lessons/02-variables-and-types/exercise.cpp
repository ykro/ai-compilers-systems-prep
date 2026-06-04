// Lesson 02 exercise - implement the TODOs until all CHECKs pass.
// Build: g++ -std=c++17 -Wall -Wextra -g -I../../include exercise.cpp -o ex && ./ex
#include "check.h"
#include <cstdint>

// TODO 1: return the result of adding x to itself using 32-bit arithmetic,
//         so that for x = 2'000'000'000 the result OVERFLOWS (wraps negative).
int32_t double_as_int32(int32_t x) {
    (void)x;   // remove this once you use x
    return 0;  // <-- replace this
}

// TODO 2: return the result of adding x to itself using 64-bit arithmetic,
//         so it does NOT overflow for the same input.
int64_t double_as_int64(int32_t x) {
    (void)x;   // remove this once you use x
    return 0;  // <-- replace this
}

int main() {
    std::printf("Lesson 02 exercise\n");
    CHECK(double_as_int32(2'000'000'000) < 0);            // proves it overflowed
    CHECK(double_as_int64(2'000'000'000) == 4'000'000'000LL); // proves it didn't
    return SUMMARY();
}
