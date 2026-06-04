// dyn_array.h — a minimal growable array of ints (like a tiny std::vector).
//
// This is the capstone: you implement/own raw heap memory (new[]/delete[]),
// which is exactly where memory bugs live. There is a PLANTED bug in
// dyn_array.cpp — your job (see BUGHUNT.md) is to find it with AddressSanitizer
// and fix it.
#pragma once

#include <cstddef>   // std::size_t

// A dynamic array that owns a heap buffer and grows as you push.
class DynArray {
public:
    DynArray();                 // start empty with a small capacity
    ~DynArray();                // free the heap buffer (RAII)

    // Non-copyable for simplicity (copying raw owning pointers is its own
    // lesson). Deleting these prevents accidental double-frees.
    DynArray(const DynArray&) = delete;
    DynArray& operator=(const DynArray&) = delete;

    void push_back(int value);  // append; grows the buffer when full
    int  at(std::size_t i) const;  // read element i (0-based)

    std::size_t size() const { return size_; }
    std::size_t capacity() const { return cap_; }

private:
    void grow();                // double the capacity, copy elements over

    int*        data_ = nullptr;  // the heap buffer
    std::size_t size_ = 0;        // number of elements in use
    std::size_t cap_  = 0;        // allocated capacity
};
