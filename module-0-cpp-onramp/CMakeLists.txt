cmake_minimum_required(VERSION 3.16)
project(cpp_onramp LANGUAGES CXX)

# --- Standard & warnings --------------------------------------------------
# C++17 everywhere; we want the modern, safe features (smart pointers, etc.).
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Treat the warnings the internship cares about as loud as possible.
if(MSVC)
  add_compile_options(/W4 /Zi)
else()
  add_compile_options(-Wall -Wextra -g)
endif()

# Every program can #include "check.h".
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

# Helper: declare a single-file program named <target> from <src>.
function(onramp_program target src)
  add_executable(${target} ${src})
endfunction()

# --- Lesson 01: setup & the compilation model -----------------------------
onramp_program(lesson01_hello      lessons/01-setup-and-compilation/hello.cpp)

# --- Lesson 02: variables, types & the stack ------------------------------
onramp_program(lesson02_example    lessons/02-variables-and-types/example.cpp)
onramp_program(lesson02_exercise   lessons/02-variables-and-types/exercise.cpp)

# --- Lesson 03: pointers & references -------------------------------------
onramp_program(lesson03_example    lessons/03-pointers-and-references/example.cpp)
onramp_program(lesson03_exercise   lessons/03-pointers-and-references/exercise.cpp)

# --- Lesson 04: memory, the heap & why bugs happen ------------------------
onramp_program(lesson04_example    lessons/04-memory-and-bugs/example.cpp)
onramp_program(lesson04_exercise   lessons/04-memory-and-bugs/exercise.cpp)

# --- Lesson 05: RAII & smart pointers -------------------------------------
onramp_program(lesson05_example    lessons/05-raii-and-smart-pointers/example.cpp)
onramp_program(lesson05_exercise   lessons/05-raii-and-smart-pointers/exercise.cpp)

# --- Lesson 06: functions, const & headers (separate compilation) ---------
add_executable(lesson06_example
  lessons/06-functions-const-headers/example.cpp
  lessons/06-functions-const-headers/mathutils.cpp)
add_executable(lesson06_exercise
  lessons/06-functions-const-headers/exercise.cpp
  lessons/06-functions-const-headers/mathutils.cpp)

# --- Lesson 07: capstone — a dynamic array (with a bug to catch) ----------
add_executable(lesson07_capstone
  lessons/07-capstone-dynamic-array/main.cpp
  lessons/07-capstone-dynamic-array/dyn_array.cpp)
