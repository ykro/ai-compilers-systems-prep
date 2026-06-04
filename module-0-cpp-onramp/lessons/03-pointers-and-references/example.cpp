// Lesson 03 example - addresses, dereferencing, and references.
#include <iostream>

int main() {
    int x = 42;
    int* p = &x;                 // p = address of x

    std::cout << "x      = " << x  << "\n";
    std::cout << "&x     = " << p  << "   (an address)\n";
    std::cout << "*p     = " << *p << "   (value at that address)\n";

    *p = 100;                    // write through the pointer
    std::cout << "after *p=100, x = " << x << "   (x changed via the pointer)\n";

    int& r = x;                  // r is an alias for x (a reference)
    r += 1;
    std::cout << "after r+=1,   x = " << x << "   (x changed via the reference)\n";

    int* nothing = nullptr;      // points to nothing; dereferencing it would crash
    std::cout << "nullptr is "  << (nothing == nullptr ? "null (don't deref!)" : "?") << "\n";

    return 0;
}
