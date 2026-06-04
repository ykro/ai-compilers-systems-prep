#ifndef DYN_ARRAY_H
#define DYN_ARRAY_H

#include <cstddef>

// A minimal growable array of ints, implemented by hand with new[]/delete[]
// so you can see what std::vector does for you under the hood.
//
// It compiles and its tests pass... but there is exactly ONE memory bug hiding
// in dyn_array.cpp. Your job (see BUGHUNT.md) is to find it with AddressSanitizer
// and a debugger, then fix it. This is a tiny version of what Project 1 automates.
class DynArray {
public:
    DynArray();
    ~DynArray();

    // No copying for this exercise (keeps the focus on the bug, not copy semantics).
    DynArray(const DynArray&) = delete;
    DynArray& operator=(const DynArray&) = delete;

    void   push_back(int value);
    int    get(std::size_t i) const;
    std::size_t size() const { return size_; }

private:
    void grow();

    int*        data_;
    std::size_t size_;
    std::size_t capacity_;
};

#endif // DYN_ARRAY_H
