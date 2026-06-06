# Bug hunt

## Step 1 — Build and run normally
```bash
cmake -S ../.. -B ../../build && cmake --build ../../build
../../build/lesson07_capstone
```
Or compile this lesson directly:
```bash
g++ -std=c++17 -Wall -Wextra -g -I../../include main.cpp dyn_array.cpp -o cap
./cap
```
You'll probably see the CHECKs **pass**. Tempting to call it done. Don't.

## Step 2 — Run under AddressSanitizer
```bash
g++ -std=c++17 -g -fsanitize=address -I../../include main.cpp dyn_array.cpp -o cap_asan
./cap_asan
```
ASan should print a `heap-buffer-overflow` report pointing into `push_back`.
Read it: which line writes out of bounds? Which allocation does it belong to?

## Step 3 — (Optional) confirm with a debugger
```bash
gdb ./cap_asan       # or: lldb ./cap_asan
```
Set a breakpoint in `push_back`, step through, and watch `size_` and `capacity_`
as elements are added. Find the moment `size_ == capacity_` but no growth happens.

## Step 4 — Fix it
Edit `dyn_array.cpp`. The growth condition is off by one. Fix it so the buffer
grows *before* writing when it is full. Rebuild under ASan and confirm a clean run.

## Step 5 — Reflect
- The functional tests passed even though memory was corrupted. Why is that dangerous?
- `std::vector` would have made this bug impossible. When is hand-rolling worth it?
- You just did, by hand, what Project 1's dynamic-analysis tools do automatically.
  That's the bridge into the internship work.
