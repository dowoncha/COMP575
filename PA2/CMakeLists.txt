cmake_minimum_required(VERSION 3.2)
project(Rasterizer)

set(Rasterizer_VERSION_MAJOR 1)
set(Rasterizer_VERSION_MAJOR 0)

add_definitions("-Wall -std=c++11")

include_directories(lib/glm)
include_directories(src)

add_subdirectory(tools/googletest)

enable_testing()

include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

add_library(RAST_LIBRARY src/Renderer.cpp
                         src/Scene.cpp
                         src/Barycentric.cpp
           )
#target_compile_features(RAST_LIBRARY PRIVATE cxx_range_for)

################################################################################
# Tests
################################################################################
#add_executable(rasterizer_unittest tests/rasterizer_unittest.cpp)
#target_link_libraries(rasterizer_unittest gtest gtest_main RAST_LIBRARY)

################################################################################
# Executables
################################################################################
add_executable(rasterizer apps/main.cpp)
target_link_libraries(rasterizer RAST_LIBRARY)
target_compile_features(rasterizer PRIVATE cxx_range_for)
