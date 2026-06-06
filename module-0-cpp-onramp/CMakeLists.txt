# Module 0 — C/C++ onramp. Builds every lesson, example, exercise, and the capstone.
# CI (.github/workflows/module-0-ci.yml) builds all targets with -Wall -Wextra -Werror.
cmake_minimum_required(VERSION 3.16)
project(cpp_onramp CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
add_compile_options(-Wall -Wextra -g)

include_directories(${CMAKE_SOURCE_DIR}/include)

# add_lesson(<target_name> <source files...>)
# Small helper so the list below stays readable. Each call makes one executable.
function(add_lesson name)
    add_executable(${name} ${ARGN})
endfunction()

# --- single-file programs (each has its own main) ---
add_lesson(lesson01_hello      lessons/01-setup-and-compilation/hello.cpp)

add_lesson(lesson02_example    lessons/02-variables-and-types/example.cpp)
add_lesson(lesson02_exercise   lessons/02-variables-and-types/exercise.cpp)

add_lesson(lesson03_example    lessons/03-pointers-and-references/example.cpp)
add_lesson(lesson03_exercise   lessons/03-pointers-and-references/exercise.cpp)

add_lesson(lesson04_example    lessons/04-memory-and-bugs/example.cpp)
add_lesson(lesson04_exercise   lessons/04-memory-and-bugs/exercise.cpp)

add_lesson(lesson05_example    lessons/05-raii-and-smart-pointers/example.cpp)
add_lesson(lesson05_exercise   lessons/05-raii-and-smart-pointers/exercise.cpp)

add_lesson(lesson06_example    lessons/06-functions-const-headers/example.cpp
                               lessons/06-functions-const-headers/mathutils.cpp)
add_lesson(lesson06_exercise   lessons/06-functions-const-headers/exercise.cpp
                               lessons/06-functions-const-headers/mathutils.cpp)

# --- capstone: multiple source files linked into one program ---
add_lesson(lesson07_capstone   lessons/07-capstone-dynamic-array/main.cpp
                               lessons/07-capstone-dynamic-array/dyn_array.cpp)
