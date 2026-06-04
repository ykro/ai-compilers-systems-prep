// Lesson 04 example - a deliberate heap buffer overflow.
// Run it TWO ways and compare:
//   g++ -std=c++17 -g example.cpp -o demo && ./demo                 # may "work" (bad!)
//   g++ -std=c++17 -g -fsanitize=address example.cpp -o demo && ./demo  # ASan catches it
#include <iostream>

int main() {
    int* arr = new int[3];        // valid indices are 0, 1, 2
    for (int i = 0; i < 3; ++i) arr[i] = i * 10;

    std::cout << "arr[0..2] = " << arr[0] << " " << arr[1] << " " << arr[2] << "\n";

    // BUG (on purpose): index 3 is one past the end -> heap buffer overflow.
    std::cout << "reading arr[3] (out of bounds!): " << arr[3] << "\n";

    delete[] arr;
    return 0;
}
