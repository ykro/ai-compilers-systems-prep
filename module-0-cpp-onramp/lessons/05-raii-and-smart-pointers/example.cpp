// example.cpp — RAII, std::unique_ptr, std::vector, std::string.
//
// Build & run:
//   g++ -std=c++17 -Wall -Wextra -g example.cpp -o example && ./example
//
// RAII = "Resource Acquisition Is Initialization". The idea: tie a resource
// (heap memory, a file, a lock) to an object's lifetime. The constructor grabs
// it; the DESTRUCTOR releases it automatically when the object goes out of
// scope. You stop writing `delete` by hand — and stop leaking.

#include <iostream>
#include <memory>     // std::unique_ptr, std::make_unique
#include <string>     // std::string
#include <vector>     // std::vector

// A tiny class that prints when it is created and destroyed, so you can SEE
// RAII happen. The destructor (~Noisy) runs automatically at end of scope.
struct Noisy {
    std::string name;
    explicit Noisy(std::string n) : name(std::move(n)) {
        std::cout << "  + constructed " << name << "\n";
    }
    ~Noisy() {
        std::cout << "  - destroyed   " << name << "\n";
    }
};

int main() {
    std::cout << "enter scope\n";
    {
        Noisy a("a (on the stack)");      // destroyed at the closing brace

        // unique_ptr owns a heap object and deletes it for you. No `delete`.
        std::unique_ptr<Noisy> b = std::make_unique<Noisy>("b (on the heap)");
        std::cout << "  using " << b->name << "\n";
    } // <- here, b's destructor frees the heap Noisy, then a's runs. Automatic.
    std::cout << "left scope (everything above was cleaned up)\n\n";

    // std::vector — a growable array that owns its memory (no new[]/delete[]).
    std::vector<int> v = {1, 2, 3};
    v.push_back(4);
    std::cout << "vector size " << v.size() << ", back = " << v.back() << "\n";
    long total = 0;
    for (int x : v) total += x;          // range-for, like Python's for x in v
    std::cout << "vector sum  " << total << "\n";

    // std::string — owns its characters; grows as needed. No manual buffers.
    std::string s = "systems";
    s += " programming";
    std::cout << "string \"" << s << "\" has length " << s.size() << "\n";

    return 0;
}
