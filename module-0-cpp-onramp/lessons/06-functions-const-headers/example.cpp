// example.cpp — the four ways to pass arguments, plus using a header.
//
// Build & run (note: TWO .cpp files compiled and linked together):
//   g++ -std=c++17 -Wall -Wextra -g example.cpp mathutils.cpp -o example && ./example
//
// Or via CMake from the project root: ./build/lesson06_example

#include <iostream>
#include <vector>
#include "mathutils.h"     // we can call mathutils::square etc. thanks to this

// 1) By value — `n` is a copy. Mutating it does nothing to the caller.
void by_value(int n) { n += 1000; (void)n; }

// 2) By reference — `n` aliases the caller's variable; changes stick.
void by_reference(int& n) { n += 1; }

// 3) By const reference — no copy, and a compile-time promise not to modify.
//    The default for "read a big object cheaply".
long length_of(const std::vector<int>& v) { return static_cast<long>(v.size()); }

// 4) By pointer — like a reference, but may be null, so check first.
void by_pointer(int* n) { if (n) *n -= 1; }

int main() {
    int x = 5;

    by_value(x);
    std::cout << "after by_value:     x = " << x << "  (unchanged)\n";

    by_reference(x);
    std::cout << "after by_reference: x = " << x << "  (now 6)\n";

    by_pointer(&x);
    std::cout << "after by_pointer:   x = " << x << "  (back to 5)\n";

    by_pointer(nullptr);   // safe: the function checks for null
    std::cout << "by_pointer(nullptr) was a safe no-op\n\n";

    // Using the separately-compiled mathutils functions:
    std::vector<int> data = {3, 1, 4, 1, 5, 9};
    std::cout << "square(7)      = " << mathutils::square(7) << "\n";
    std::cout << "sum(data)      = " << mathutils::sum(data) << "\n";
    std::cout << "length_of(data)= " << length_of(data) << "\n";
    std::cout << "clamp(42,0,10) = " << mathutils::clamp(42, 0, 10) << "\n";

    return 0;
}
