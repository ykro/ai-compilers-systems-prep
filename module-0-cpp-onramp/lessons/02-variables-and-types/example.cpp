// Lesson 02 example - integers overflow, and values are copied.
#include <iostream>
#include <cstdint>

int main() {
    // 1) Overflow: int is ~32 bits. This wraps around instead of growing.
    int32_t big = 2'000'000'000;
    int32_t sum = big + big;            // overflow! not 4 billion
    std::cout << "2e9 + 2e9 as int32 = " << sum << "  (wrapped, not 4000000000)\n";

    // Use a wider type to hold the real value:
    int64_t safe = static_cast<int64_t>(big) + big;
    std::cout << "same, but as int64  = " << safe << "\n";

    // 2) Value semantics: copies are independent.
    int a = 10;
    int b = a;        // b is a COPY of a
    b = 99;           // changing b does not touch a
    std::cout << "a=" << a << " b=" << b << "  (a unchanged: this is a copy)\n";

    return 0;
}
