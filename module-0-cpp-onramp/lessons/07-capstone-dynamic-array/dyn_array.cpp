#include "dyn_array.h"

// Start with a small, REAL heap buffer so the array is usable immediately.
DynArray::DynArray() : data_(new int[4]), size_(0), capacity_(4) {}

DynArray::~DynArray() {
    delete[] data_;          // release the heap buffer (RAII would do this for us)
}

void DynArray::grow() {
    std::size_t new_cap = capacity_ * 2;
    int* new_data = new int[new_cap];
    for (std::size_t i = 0; i < size_; ++i) {
        new_data[i] = data_[i];     // copy existing elements
    }
    delete[] data_;
    data_ = new_data;
    capacity_ = new_cap;
}

void DynArray::push_back(int value) {
    // BUG (on purpose): the buffer is FULL when size_ == capacity_, and we must
    // grow BEFORE writing. The comparison below is off by one (`>` instead of
    // `>=`), so exactly on the boundary we skip the grow and then write to
    // data_[capacity_] -- one slot past the end of the heap buffer.
    // That is a heap buffer overflow.  <-- find & fix me (see BUGHUNT.md)
    if (size_ > capacity_) {
        grow();
    }
    data_[size_] = value;
    size_++;
}

int DynArray::get(std::size_t i) const {
    return data_[i];
}
