// mathutils.h — DECLARATIONS only (the interface).
//
// A header says *what* exists; the matching .cpp says *how* it works. Code that
// wants these functions #includes this header and is compiled separately, then
// the linker joins everything. This is "separate compilation".
#pragma once          // include this header at most once per translation unit

#include <cstddef>    // std::size_t
#include <vector>

namespace mathutils {

// Pass by value: `n` is a copy; callers' variable is untouched.
int square(int n);

// Pass by const reference: no copy of the (possibly large) vector, and the
// `const` promises we won't modify it. This is the default for "read a big
// object".
long sum(const std::vector<int>& v);

// Clamp `x` into [lo, hi].
int clamp(int x, int lo, int hi);

}  // namespace mathutils
