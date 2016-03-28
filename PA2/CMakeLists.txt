cmake_minimum_required(VERSION 3.2)
project(Rasterizer)

set(Rasterizer_VERSION_MAJOR 1)
set(Rasterizer_VERSION_MAJOR 0)

add_definitions("-std=c++11")

include_directories(lib/glm)
include_directories(src)

add_library(RAST_LIBRARY src/Renderer.cpp
                         src/Scene.cpp
                         src/Barycentric.cpp
           )

add_executable(rasterizer src/main.cpp)
target_link_libraries(rasterizer RAST_LIBRARY)
target_compile_features(rasterizer PRIVATE cxx_range_for)
