// exercise.cpp — heap memory, your turn.
//
// Compiles as shipped; CHECKs fail until you implement the TODOs.
//   g++ -std=c++17 -Wall -Wextra -g -I../../include exercise.cpp -o exercise && ./exercise
//
// THEN, once the CHECKs pass, prove you have no leaks or overflows by running
// it under a sanitizer (it should still pass, with no ASan report):
//   g++ -std=c++17 -g -fsanitize=address,undefined -I../../include exercise.cpp -o ex_asan && ./ex_asan

#include <cstddef>      // std::size_t
#include "check.h"

// TODO 1: allocate an array of `n` ints on the heap, fill it so that
// element i == i*i (0,1,4,9,...), and return the pointer. The CALLER owns it
// and will delete[] it. Hint: `new int[n]`.
int* make_squares(std::size_t n) {
    (void)n;
    return nullptr;   // <- replace
}

// TODO 2: return the sum of the first `n` elements of `data`.
// Read only indices 0..n-1 — reading data[n] would be a buffer overflow.
long sum(const int* data, std::size_t n) {
    (void)data;
    (void)n;
    return 0;   // <- replace
}

int main() {
    const std::size_t n = 5;
    int* squares = make_squares(n);

    CHECK(squares != nullptr);
    if (squares != nullptr) {
        CHECK_EQ(squares[0], 0);
        CHECK_EQ(squares[2], 4);
        CHECK_EQ(squares[4], 16);
        CHECK_EQ(sum(squares, n), 0 + 1 + 4 + 9 + 16);

        // You allocated it, so you must free it. Forgetting this line is a
        // leak — run under ASan to watch it get reported.
        delete[] squares;
    }

    return check_report();
}
