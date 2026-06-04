# Running this onramp on Google Colab

You do **not** need to install anything locally. A Colab notebook is a Linux machine with `g++` already installed.

> Note: for C/C++ you do **not** need a GPU runtime. The default CPU runtime is fine. (You'll switch to a GPU runtime later, for Project 2.)

## 1. Get the files into Colab

Easiest: clone your repo.

```python
!git clone https://github.com/ykro/ai-compilers-systems-prep.git
%cd ai-compilers-systems-prep/module-0-cpp-onramp
```

## 2. Compile and run a single file

In a code cell, lines starting with `!` run in the shell:

```python
!g++ -std=c++17 -Wall -Wextra -g -Iinclude lessons/02-variables-and-types/example.cpp -o /tmp/prog
!/tmp/prog
```

## 3. Run with sanitizers (Lesson 04 onward)

```python
!g++ -std=c++17 -g -Iinclude -fsanitize=address,undefined lessons/04-memory-and-bugs/example.cpp -o /tmp/demo
!/tmp/demo
```

When a memory bug fires, AddressSanitizer prints a detailed report right in the cell output.

## 4. Build everything with CMake (optional)

```python
!apt-get -qq install -y cmake > /dev/null
!cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug
!cmake --build build -j
!./build/lesson07_capstone
```

## 5. Don't lose your work

Colab disconnects after inactivity. Either keep pushing to GitHub, or mount Drive:

```python
from google.colab import drive
drive.mount('/content/drive')
```

## Trade-off to keep in mind

Colab works, but you miss out on a real **IDE + step debugger**, which is part of what this module is teaching. If you have a laptop, prefer building locally with CLion or VS Code + clangd, and use Colab as a backup.
