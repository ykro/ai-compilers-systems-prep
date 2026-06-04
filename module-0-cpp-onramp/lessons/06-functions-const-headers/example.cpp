// Lesson 06 example - pass-by-value vs reference vs const-ref, across files.
#include <iostream>
#include <vector>
#include "mathutils.h"

void add_one_by_value(int n)  { n += 1; (void)n; }   // changes the local copy only
void add_one_by_ref(int& n)   { n += 1; }            // changes the caller's variable

int main() {
    int x = 10;
    add_one_by_value(x);
    std::cout << "after by_value: x = " << x << "  (unchanged)\n";
    add_one_by_ref(x);
    std::cout << "after by_ref:   x = " << x << "  (changed)\n";

    std::vector<int> v = {3, 1, 4, 1, 5};
    std::cout << "sum (no copy, const&) = " << sum(v) << "\n";   // defined in mathutils.cpp
    return 0;
}
