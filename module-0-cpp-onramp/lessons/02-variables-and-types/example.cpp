// example.cpp — fixed-size integers, overflow, and value (copy) semantics.
//
// Build & run:
//   g++ -std=c++17 -Wall -Wextra -g example.cpp -o example && ./example
//
// Two surprises for people coming from Java/Python:
//   1. Integers have a FIXED size and WRAP AROUND when they overflow.
//   2. Assigning a value COPIES it. Two variables are not the same object.

#include <cstdint>    // int32_t, uint8_t, ... — sized integer types
#include <iostream>
#include <limits>     // std::numeric_limits

int main() {
    // --- Fixed-size types -------------------------------------------------
    // In Python, ints grow without limit. In C++ a type has a fixed width and
    // a fixed range. Pick the size deliberately.
    std::cout << "int32_t  range: " << std::numeric_limits<int32_t>::min()
              << " .. " << std::numeric_limits<int32_t>::max() << "\n";
    std::cout << "uint8_t  range: 0 .. "
              << static_cast<int>(std::numeric_limits<uint8_t>::max()) << "\n\n";

    // --- Overflow / wraparound -------------------------------------------
    // An 8-bit unsigned value holds 0..255. Add past the top and it wraps.
    // This is exactly the class of bug security tools hunt for.
    uint8_t small = 250;
    for (int i = 0; i < 8; ++i) {
        std::cout << "small = " << static_cast<int>(small) << "\n";
        small = static_cast<uint8_t>(small + 1);   // 255 -> 0, silently
    }
    std::cout << "(it wrapped from 255 back to 0)\n\n";

    // --- Value semantics: assignment copies ------------------------------
    // In Java/Python, `b = a` for an object makes b point at a's object.
    // In C++, for a plain value type, `b = a` COPIES the bits.
    int a = 10;
    int b = a;     // b is an independent copy
    b = 99;        // changing b does NOT touch a
    std::cout << "a = " << a << ", b = " << b
              << "   <- a is unchanged; b was a separate copy\n";

    return 0;
}
