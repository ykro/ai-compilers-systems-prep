// Capstone - exercise the hand-rolled DynArray.
//
// Run it BOTH ways:
//   1) normal build: it may print fine and even pass the CHECKs (scary - the
//      memory is corrupt but the program "looks" OK; this is undefined behavior).
//   2) with AddressSanitizer: the bug is caught at the exact line.
//      g++ -std=c++17 -g -fsanitize=address -I../../include main.cpp dyn_array.cpp -o cap && ./cap
//
// See BUGHUNT.md for the full task.
#include "check.h"
#include "dyn_array.h"

int main() {
    std::printf("Capstone: DynArray bug hunt\n");

    DynArray a;
    for (int i = 0; i < 8; ++i) a.push_back(i * 11);  // crosses growth boundaries

    CHECK(a.size() == 8);
    CHECK(a.get(0) == 0);
    CHECK(a.get(7) == 77);

    int s = SUMMARY();
    std::printf("\nNow rebuild with -fsanitize=address and run again.\n");
    std::printf("If ASan reports nothing, you've fixed the bug.\n");
    return s;
}
