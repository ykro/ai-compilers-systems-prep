// exercise.cpp — your turn.
//
// This file COMPILES as shipped, but the CHECK assertions in main() FAIL
// until you implement the TODOs. Build & run:
//   g++ -std=c++17 -Wall -Wextra -g -I../../include exercise.cpp -o exercise && ./exercise
// (or from the project root, after `cmake --build build`: ./build/lesson02_exercise)
//
// Goal: feel fixed-size integers and value semantics in your own code.

#include <cstdint>
#include "check.h"

// TODO 1: return the result of adding 1 to an 8-bit unsigned value `x`,
// WRAPPING AROUND (so wrap_add_u8(255) must return 0, not 256).
// Hint: do the arithmetic in uint8_t and let it wrap, or use % 256.
uint8_t wrap_add_u8(uint8_t x) {
    (void)x;        // silences the "unused parameter" warning until you use it
    return 0;       // <- replace this
}

// TODO 2: swap the values of `a` and `b` THROUGH COPIES (by value).
// Because C++ copies, taking parameters by value here would NOT affect the
// caller — so these are references (`int&`). Fill in the body so the caller's
// variables actually swap. Hint: you need a temporary.
void swap_ints(int& a, int& b) {
    (void)a;
    (void)b;
    // <- implement the swap
}

int main() {
    // Fixed-size wraparound
    CHECK_EQ(wrap_add_u8(0), 1);
    CHECK_EQ(wrap_add_u8(254), 255);
    CHECK_EQ(wrap_add_u8(255), 0);     // the wrap

    // Value semantics: a plain copy must not alias the original
    int original = 7;
    int copy = original;
    copy = 100;
    CHECK_EQ(copy, 100);              // the copy changed...
    CHECK_EQ(original, 7);             // ...but the original must be untouched

    // Reference swap must change the caller's variables
    int x = 1, y = 2;
    swap_ints(x, y);
    CHECK_EQ(x, 2);
    CHECK_EQ(y, 1);

    return check_report();
}
