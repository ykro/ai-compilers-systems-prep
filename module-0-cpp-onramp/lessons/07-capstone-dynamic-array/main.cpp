// main.cpp — exercise the DynArray.
//
// Plain build & run (likely "works", but contains undefined behavior):
//   g++ -std=c++17 -Wall -Wextra -g main.cpp dyn_array.cpp -o capstone && ./capstone
//   (or: ./build/lesson07_capstone after a CMake build)
//
// THE BUG HUNT — compile with AddressSanitizer and run:
//   g++ -std=c++17 -g -fsanitize=address,undefined main.cpp dyn_array.cpp -o capstone_asan
//   ./capstone_asan
// ASan aborts with a heap-buffer-overflow report pointing into dyn_array.cpp.
// Read BUGHUNT.md, fix the planted bug, and confirm the ASan run is clean.

#include <cstdio>
#include "dyn_array.h"

int main() {
    DynArray a;

    // Push enough values to force the buffer to grow more than once
    // (capacity starts at 2: it grows at the 3rd push, again at the 5th).
    for (int i = 1; i <= 6; ++i) {
        a.push_back(i * 10);
    }

    std::printf("size = %zu, capacity = %zu\n", a.size(), a.capacity());

    long total = 0;
    for (std::size_t i = 0; i < a.size(); ++i) {
        std::printf("a[%zu] = %d\n", i, a.at(i));
        total += a.at(i);
    }
    std::printf("sum = %ld (expected 210)\n", total);

    return 0;
}
