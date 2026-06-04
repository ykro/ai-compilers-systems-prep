// exercise.cpp — RAII & smart pointers, your turn.
//
// Compiles as shipped; CHECKs fail until you implement the TODOs.
//   g++ -std=c++17 -Wall -Wextra -g -I../../include exercise.cpp -o exercise && ./exercise
//
// The whole point: NO `new`/`delete` anywhere in your code. Use the standard
// library's owning types. Run it under ASan too — there should be no leaks,
// precisely because RAII frees everything for you.

#include <memory>
#include <numeric>     // std::accumulate
#include <string>
#include <vector>
#include "check.h"

// TODO 1: build and return a vector containing 1..n (inclusive).
// e.g. make_range(4) -> {1, 2, 3, 4}. No raw arrays.
std::vector<int> make_range(int n) {
    (void)n;
    return {};   // <- replace
}

// TODO 2: return the sum of all elements. Use a range-for or std::accumulate.
long total(const std::vector<int>& v) {
    (void)v;
    return 0;   // <- replace
}

// TODO 3: create an int on the heap via std::make_unique, holding `value`,
// and return the unique_ptr. The caller owns it; it frees automatically.
std::unique_ptr<int> boxed(int value) {
    (void)value;
    return nullptr;   // <- replace with std::make_unique<int>(value)
}

// TODO 4: return a greeting string "Hello, <who>!" using std::string.
std::string greet(const std::string& who) {
    (void)who;
    return "";   // <- replace
}

int main() {
    std::vector<int> r = make_range(4);
    CHECK_EQ(r.size(), 4u);
    if (r.size() == 4u) {
        CHECK_EQ(r.front(), 1);
        CHECK_EQ(r.back(), 4);
        CHECK_EQ(total(r), 10);
    }

    std::unique_ptr<int> p = boxed(42);
    CHECK(p != nullptr);
    if (p) CHECK_EQ(*p, 42);

    CHECK(greet("world") == "Hello, world!");

    return check_report();
}
