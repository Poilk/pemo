cmake_minimum_required(VERSION 3.20)
project(pemo)

set(CMAKE_CXX_STANDARD 11)

add_compile_options(-Werror=return-type -fPIC)

add_subdirectory(include)
add_subdirectory(src)
add_subdirectory(tests/pemo)