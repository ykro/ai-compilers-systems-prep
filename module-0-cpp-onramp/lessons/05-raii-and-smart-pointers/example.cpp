// Lesson 05 example - RAII destructors, unique_ptr, vector, string.
#include <iostream>
#include <memory>
#include <vector>
#include <string>

struct Logger {
    std::string name;
    explicit Logger(std::string n) : name(std::move(n)) {
        std::cout << "[open " << name << "]\n";
    }
    ~Logger() { std::cout << "[close " << name << "]\n"; }
};

int main() {
    {
        Logger a("A");                 // constructed here
        auto p = std::make_unique<int>(7);
        std::cout << "unique_ptr holds " << *p << "\n";
    } // <- a's destructor and p's cleanup run automatically here

    std::vector<int> v = {1, 2, 3};
    v.push_back(4);
    std::cout << "vector:";
    for (int x : v) std::cout << " " << x;   // range-for
    std::cout << "  size=" << v.size() << "\n";

    std::string s = "Universidad ";
    s += "Galileo";
    std::cout << "string: " << s << " (len " << s.size() << ")\n";
    return 0;
}
