cmake_minimum_required(VERSION 3.10)

project(Manifold VERSION 1.0)

add_subdirectory(src)

target_include_directories(manifold PUBLIC include)
