cmake_minimum_required(VERSION 3.14)
project(22s_PA03)

# Requires usage of C++20
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Configures compiler optimization (e.g. O0, O1, O2, O3, or Ofast)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O0")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Ofast")

add_executable(22s_PA03 src/main.cpp src/Girvan_Newman.cpp src/Girvan_Newman.h)