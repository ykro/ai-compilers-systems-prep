// Lesson 01 - the smallest complete C++ program.
// Compile:  g++ -std=c++17 -Wall -Wextra -g hello.cpp -o hello
// Run:      ./hello

#include <iostream>   // <-- pastes the declarations for std::cout, etc.

int main() {
    // std::cout is the output stream; << sends data to it.
    // std::endl writes a newline and flushes the buffer.
    std::cout << "Hello from a native binary." << std::endl;

    // In Python/Java the entry point is implicit or a static method.
    // In C++ it is the function named main, which returns an int:
    //   0 means success; nonzero means failure (the shell can read this).
    return 0;
}
