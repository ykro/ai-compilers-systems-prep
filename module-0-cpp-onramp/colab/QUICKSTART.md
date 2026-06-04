# Colab Quickstart (no local install)

If you can't set up a compiler locally yet, you can do the whole onramp in a
[Google Colab](https://colab.research.google.com) notebook. Colab's Linux VM
already has `g++` installed — every cell that starts with `!` runs a shell
command on that VM.

> Local setup is still recommended (you practice a real IDE + debugger), but
> Colab is a fine way to start today.

## 1. Check the compiler is there

```python
!g++ --version
!cmake --version   # usually present; if not: !apt-get -qq install -y cmake
```

## 2. Get the code

```python
# Replace with your repo URL once it's published:
!git clone https://github.com/ykro/ai-compilers-systems-prep.git
%cd ai-compilers-systems-prep/module-0-cpp-onramp
```

## 3. Compile and run a single file

There is no "run" button in C++: you compile a source file into a binary, then
run the binary.

```python
!g++ -std=c++17 -Wall -Wextra -g -Iinclude \
    lessons/02-variables-and-types/example.cpp -o /tmp/example
!/tmp/example
```

For a multi-file lesson (06 and the capstone) list every `.cpp`:

```python
!g++ -std=c++17 -Iinclude \
    lessons/07-capstone-dynamic-array/main.cpp \
    lessons/07-capstone-dynamic-array/dyn_array.cpp -o /tmp/capstone
!/tmp/capstone
```

## 4. Build everything at once with CMake (optional)

```python
!cmake -S . -B build
!cmake --build build
!./build/lesson02_example
```

## 5. Run with sanitizers (from Lesson 04 onward)

This is the important one — sanitizers are how you *see* memory bugs. Compile
with `-fsanitize=address,undefined` and run; the program aborts with an exact
report (file, line, kind of error) the moment a bug happens.

```python
!g++ -std=c++17 -g -fsanitize=address,undefined -Iinclude \
    lessons/04-memory-and-bugs/example.cpp -o /tmp/asan_demo
!/tmp/asan_demo
```

## Tips

- A non-zero exit / red output usually means a `CHECK` failed or a sanitizer
  caught a bug — read the message, it tells you the file and line.
- Colab VMs are ephemeral: anything outside the cloned repo (and `/tmp`)
  disappears when the runtime resets. Re-run the clone cell if that happens.
