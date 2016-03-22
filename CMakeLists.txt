cmake_minimum_required(VERSION 3.2)
project(Rasterizer)

set(Rasterizer_VERSION_MAJOR 1)
set(Rasterizer_VERSION_MAJOR 0)

include_directories(lib/glm)
include_directories(src)

add_library(RAST_LIBRARY src/Renderer.cpp
                         src/Scene.cpp
                         src/Light.hpp
                         src/Barycentric.cpp
           )
target_compile_features(RAST_LIBRARY PRIVATE cxx_range_for)

################################################################################
# Tests
################################################################################
#add_executable(scene_unittest tests/scene_unittest.cpp)
#target_link_libraries(scene_unittest gtest_main PA2_SRC)

################################################################################
# Executables
################################################################################
add_executable(flatshading apps/flatshading.cpp)
target_link_libraries(flatshading RAST_LIBRARY)
target_compile_features(flatshading PRIVATE cxx_range_for)

add_executable(testyellow apps/test.cpp)
target_compile_features(testyellow PRIVATE cxx_range_for)
