// Lesson 04 exercise - fix the leak and the overflow.
// Goal: runs cleanly under  -fsanitize=address,undefined  with all CHECKs passing.
// Build: g++ -std=c++17 -g -fsanitize=address,undefined -I../../include exercise.cpp -o ex && ./ex
#include "check.h"

// TODO 1: this leaks - it allocates but never frees.
//         Return the sum, but free the memory before returning.
int sum_first_n(int n) {
    int* data = new int[n];
    int total = 0;
    for (int i = 0; i < n; ++i) { data[i] = i + 1; total += data[i]; }
    // <-- something is missing here before the return
    return total;
}

// TODO 2: this writes one element past the end (buffer overflow).
//         Fix the loop bound so every write is in range.
void fill(int* arr, int len) {
    for (int i = 0; i <= len; ++i) {   // <-- look at the comparison carefully
        arr[i] = i;
    }
}

int main() {
    std::printf("Lesson 04 exercise\n");

    CHECK(sum_first_n(5) == 15);   // 1+2+3+4+5

    int* buf = new int[4];
    fill(buf, 4);
    CHECK(buf[0] == 0 && buf[3] == 3);
    delete[] buf;

    return SUMMARY();
}
