cmake_minimum_required(VERSION 3.2)
project(Rasterizer)

add_definitions("-std=c++11")

#include glut
#include_directories(lib/glm)
#use eigen
include_directories(lib/freeglut/include)
include_directories(src)

add_subdirectory(lib/freeglut)

add_executable(rasterizer src/main.cpp)
#target_link_libraries(rasterizer RAST_LIBRARY)
#target_compile_features(rasterizer PRIVATE cxx_range_for)
