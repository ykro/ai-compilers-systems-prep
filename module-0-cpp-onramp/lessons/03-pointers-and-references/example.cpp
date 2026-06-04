// example.cpp — pointers (`*`, `&`) and references.
//
// Build & run:
//   g++ -std=c++17 -Wall -Wextra -g example.cpp -o example && ./example
//
// A pointer is just a variable holding the ADDRESS of another variable.
// `&x`  = "the address of x".
// `*p`  = "the value stored at the address p holds" (dereference).
// A reference is an alias for an existing variable — no `*` needed to use it.

#include <iostream>

// Pass by pointer: the caller passes an address; we modify through it.
void add_one_via_pointer(int* p) {
    if (p == nullptr) return;   // always check before dereferencing
    *p = *p + 1;                // write through the pointer
}

// Pass by reference: `r` is an alias for the caller's variable. Cleaner, and
// it can't be null.
void add_one_via_reference(int& r) {
    r = r + 1;
}

int main() {
    int x = 41;

    // A pointer holds x's address. Print the address and the value at it.
    int* p = &x;
    std::cout << "x lives at address " << static_cast<const void*>(p) << "\n";
    std::cout << "value via *p     = " << *p << "\n";

    add_one_via_pointer(&x);    // pass the ADDRESS of x
    std::cout << "after pointer +1 = " << x << "\n";

    add_one_via_reference(x);   // pass x; the reference aliases it
    std::cout << "after ref +1     = " << x << "\n\n";

    // --- nullptr: a pointer that points at nothing ------------------------
    int* nothing = nullptr;
    std::cout << "nothing == nullptr? " << std::boolalpha
              << (nothing == nullptr) << "\n";
    // Dereferencing `nothing` here would crash — that's why the function above
    // checks for nullptr first.

    // --- a pointer can be re-aimed; a reference cannot --------------------
    int y = 100;
    p = &y;                     // p now points at y instead of x
    std::cout << "p now sees y = " << *p << "\n";

    return 0;
}
