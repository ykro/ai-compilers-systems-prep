// hello.cpp — your first C++ program.
//
// Build it by hand to *see* the compile step (there is no "run" button):
//
//     g++ -std=c++17 -Wall -Wextra -g hello.cpp -o hello
//     ./hello
//
// What just happened:
//   1. The compiler turned this text file into machine code (an object file).
//   2. The linker combined it with the C++ standard library into one binary
//      called `hello`.
//   3. `./hello` asked the OS to load and run that binary.
//
// In Java you'd run `javac` then `java Hello`; in Python you'd just `python
// hello.py`. Here the output is a standalone native executable — no runtime
// needed to launch it.

#include <iostream>   // std::cout, std::endl  (like System.out / print)

int main() {
    // std::cout is the standard output stream. `<<` feeds values into it.
    std::cout << "Hello, systems programming!\n";

    // A C++ program returns an int exit code. 0 means success; anything else
    // signals an error to whatever launched the program (the shell, a script,
    // a test runner). Try `echo $?` after running to see it.
    return 0;
}
