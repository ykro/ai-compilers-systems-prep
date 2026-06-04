// dyn_array.cpp — implementation of DynArray.
//
// >>> There is ONE planted memory bug in this file. <<<
// The program appears to run fine without sanitizers (the value of an
// out-of-bounds read is usually harmless garbage), but it IS undefined
// behavior. Compile main with AddressSanitizer and it will point you straight
// at the offending line. See BUGHUNT.md, then fix it.
#include "dyn_array.h"

DynArray::DynArray() {
    cap_  = 2;                  // start with room for 2 elements
    data_ = new int[cap_];      // heap buffer
    size_ = 0;
}

DynArray::~DynArray() {
    delete[] data_;             // give the heap memory back
}

void DynArray::grow() {
    std::size_t new_cap = cap_ * 2;
    int* new_data = new int[new_cap];

    // Copy existing elements into the bigger buffer.
    //
    // BUG: this uses `<=`, so when size_ == cap_ (which is exactly when grow()
    // is called) it reads data_[size_] — one element PAST the end of the old
    // buffer. That is a heap-buffer-overflow READ. The correct bound is `<`.
    for (std::size_t i = 0; i <= size_; ++i) {
        new_data[i] = data_[i];
    }

    delete[] data_;
    data_ = new_data;
    cap_  = new_cap;
}

void DynArray::push_back(int value) {
    if (size_ == cap_) {
        grow();                 // out of room — make the buffer bigger
    }
    data_[size_] = value;
    ++size_;
}

int DynArray::at(std::size_t i) const {
    return data_[i];            // (intentionally unchecked — keep it minimal)
}
