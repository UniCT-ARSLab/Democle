cmake_minimum_required (VERSION 2.6)
set (VERSION 0.1)
set (PACKAGE democle)
project (${PACKAGE})

find_package(Threads REQUIRED)

add_subdirectory (lib)
add_subdirectory (examples)

add_compile_options(-g)

