cmake_minimum_required(VERSION 3.25)
project(chess)

set(CMAKE_CXX_STANDARD 20)

add_executable(chess main.cpp implementation/game.cpp)

target_compile_features(chess PRIVATE cxx_range_for)
