// example.cpp — the heap (`new`/`delete`) and the classic memory bugs.
//
// Clean run (no bug, builds via CMake as lesson04_example):
//   g++ -std=c++17 -Wall -Wextra -g example.cpp -o example && ./example
//
// SEE A BUG GET CAUGHT — compile with the bug enabled AND a sanitizer:
//   g++ -std=c++17 -g -DSHOW_BUG=1 -fsanitize=address,undefined example.cpp -o demo
//   ./demo
// AddressSanitizer aborts at the exact line with a report (heap-use-after-free
// / heap-buffer-overflow). That report is the whole point of this lesson — it
// is the same machinery Project 1 (Sentinel) builds tooling around.
//
// Set SHOW_BUG to 1, 2, or 3 to pick which bug to trigger.

#include <cstdio>

int main() {
    // --- The heap: you ask for memory, and you must give it back ----------
    // `new` allocates on the heap and returns a pointer. Unlike a stack
    // variable, heap memory lives until YOU `delete` it. Forgetting to is a
    // LEAK; using it after delete is USE-AFTER-FREE; writing past its end is a
    // BUFFER OVERFLOW.
    int* heap_value = new int(123);          // one int on the heap
    std::printf("heap_value = %d\n", *heap_value);
    delete heap_value;                       // give it back — required
    heap_value = nullptr;                    // good habit: avoid dangling use

    int* arr = new int[5];                   // an array of 5 ints on the heap
    for (int i = 0; i < 5; ++i) arr[i] = i * 10;
    std::printf("arr[4] = %d\n", arr[4]);

#if SHOW_BUG == 1
    // BUG 1 — heap-buffer-overflow: index 5 is one past the end (valid: 0..4).
    std::printf("arr[5] = %d\n", arr[5]);    // ASan: heap-buffer-overflow
#endif

#if SHOW_BUG == 2
    // BUG 2 — heap-use-after-free: read memory we already returned.
    delete[] arr;
    std::printf("arr[0] after free = %d\n", arr[0]);  // ASan: use-after-free
    arr = nullptr;
#endif

    delete[] arr;                            // the correct cleanup

#if SHOW_BUG == 3
    // BUG 3 — a leak: allocate and never delete. ASan's leak check reports it
    // at program exit (LeakSanitizer, part of ASan on Linux).
    int* leaked = new int[1000];
    leaked[0] = 1;
    std::printf("leaked[0] = %d (and never freed)\n", leaked[0]);
#endif

    std::printf("done\n");
    return 0;
}
