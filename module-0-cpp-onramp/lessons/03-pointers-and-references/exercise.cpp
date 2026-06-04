// exercise.cpp — pointers & references, your turn.
//
// Compiles as shipped; the CHECKs fail until you implement the TODOs.
//   g++ -std=c++17 -Wall -Wextra -g -I../../include exercise.cpp -o exercise && ./exercise
//   (or: ./build/lesson03_exercise after a CMake build)

#include "check.h"

// TODO 1: through the pointer `p`, set the pointed-to int to `value`.
// If `p` is null, do nothing (never dereference a null pointer).
void set_through_pointer(int* p, int value) {
    (void)p;
    (void)value;
    // <- implement
}

// TODO 2: return the larger of the two ints, BY REFERENCE, so the caller can
// modify the original through the returned reference. (Return type is int&.)
// Hint: return a or b, not a copy.
int& max_ref(int& a, int& b) {
    (void)b;
    return a;   // <- fix so it returns whichever is larger
}

// TODO 3: return true if `p` is safe to dereference (i.e. not null).
bool is_valid(const int* p) {
    (void)p;
    return true;   // <- replace with the real check
}

int main() {
    int n = 0;
    set_through_pointer(&n, 42);
    CHECK_EQ(n, 42);

    int nothing_changed = 7;
    set_through_pointer(nullptr, 999);   // must be a safe no-op
    CHECK_EQ(nothing_changed, 7);

    int a = 3, b = 9;
    // max_ref returns a reference to b (the larger); writing through it edits b.
    max_ref(a, b) = 100;
    CHECK_EQ(b, 100);
    CHECK_EQ(a, 3);

    int real = 5;
    CHECK(is_valid(&real) == true);
    CHECK(is_valid(nullptr) == false);

    return check_report();
}
