// Lesson 03 exercise - implement the TODOs until all CHECKs pass.
// Build: g++ -std=c++17 -Wall -Wextra -g -I../../include exercise.cpp -o ex && ./ex
#include "check.h"

// TODO 1: swap the two ints THROUGH the pointers.
//         After the call, *a and *b should have exchanged values.
void swap_ints(int* a, int* b) {
    // <-- implement using *a and *b
    (void)a; (void)b;  // remove these; they only silence "unused" warnings
}

// TODO 2: increment the referenced int by one.
void increment(int& n) {
    // <-- implement
    (void)n;  // remove this
}

int main() {
    std::printf("Lesson 03 exercise\n");

    int x = 1, y = 2;
    swap_ints(&x, &y);
    CHECK(x == 2);
    CHECK(y == 1);

    int c = 41;
    increment(c);
    CHECK(c == 42);

    return SUMMARY();
}
