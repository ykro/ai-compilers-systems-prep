// exercise.cpp — functions, const & headers, your turn.
//
// Compiles & links as shipped (with mathutils.cpp); CHECKs fail until you do
// the TODOs:
//   g++ -std=c++17 -Wall -Wextra -g -I../../include exercise.cpp mathutils.cpp -o exercise && ./exercise
//   (or: ./build/lesson06_exercise after a CMake build)

#include <vector>
#include "check.h"
#include "mathutils.h"     // you'll call one function from the header too

// TODO 1: add 10 to the caller's variable, THROUGH A REFERENCE (changes stick).
void add_ten(int& n) {
    (void)n;
    // <- implement
}

// TODO 2: through the pointer, double the pointed-to value. If `p` is null,
// do nothing.
void double_in_place(int* p) {
    (void)p;
    // <- implement
}

// TODO 3: take the vector by CONST REFERENCE (no copy, read-only) and return
// the count of elements strictly greater than `threshold`.
int count_above(const std::vector<int>& v, int threshold) {
    (void)v;
    (void)threshold;
    return 0;   // <- implement
}

int main() {
    int a = 5;
    add_ten(a);
    CHECK_EQ(a, 15);

    int b = 21;
    double_in_place(&b);
    CHECK_EQ(b, 42);

    int unchanged = 9;
    double_in_place(nullptr);   // must be a safe no-op
    CHECK_EQ(unchanged, 9);

    std::vector<int> v = {1, 5, 2, 8, 3, 9};
    CHECK_EQ(count_above(v, 4), 3);   // 5, 8, 9

    // And confirm the header/.cpp split works: call into mathutils.
    CHECK_EQ(mathutils::square(6), 36);

    return check_report();
}
