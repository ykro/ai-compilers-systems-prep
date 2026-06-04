// Lesson 05 exercise - eliminate manual memory management.
// Build: g++ -std=c++17 -Wall -Wextra -g -fsanitize=address -I../../include exercise.cpp -o ex && ./ex
#include "check.h"
#include <memory>
#include <vector>
#include <numeric>

// TODO 1: this version leaks. Rewrite the body to use std::vector<int>
//         (no new/delete) and return the sum. Behavior must be identical.
int sum_first_n(int n) {
    int* data = new int[n];                 // <-- replace with std::vector<int>
    for (int i = 0; i < n; ++i) data[i] = i + 1;
    int total = 0;
    for (int i = 0; i < n; ++i) total += data[i];
    return total;                           // <-- leaks: data never freed
}

// TODO 2: return a unique_ptr<int> that owns a heap int equal to value.
//         (Use std::make_unique.)
std::unique_ptr<int> make_owned_int(int value) {
    (void)value;
    return nullptr;   // <-- replace
}

int main() {
    std::printf("Lesson 05 exercise\n");
    CHECK(sum_first_n(5) == 15);
    auto p = make_owned_int(42);
    CHECK(p != nullptr);
    CHECK(p && *p == 42);
    return SUMMARY();
}
