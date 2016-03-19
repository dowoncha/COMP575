cmake_minimum_required(VERSION 3.2)
project(Rasterizer)

set(Rasterizer_VERSION_MAJOR 1)
set(Rasterizer_VERSION_MAJOR 0)

###############################################################################
# Find CLANG package
#find_package(LLVM REQUIRED CONFIG)
#message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
#message(STATUS "Using LLVMConfig.cmake in: ${LLVM_DIR}")

# Add GL packages
find_package(OpenGL REQUIRED)
if (NOT OPENGL_FOUND)
  message("ERROR: OpenGL not found")
endif(NOT OPENGL_FOUND)
message(STATUS "Found OpenGL")
set(GL_LIBRARY GL GLU X11)

#include_directories(${LLVM_INCLUDE_DIRS})
#add_definitions(${LLVM_DEFINITIONS})

include_directories(lib/freeglut/include)
include_directories(lib/glm)
include_directories(${GTEST_ROOT}/include)

add_subdirectory(lib/freeglut)
add_subdirectory(tools/googletest)

include_directories(src)

add_library(PA2_SRC src/Renderer.cpp
                    src/Scene.cpp)
target_compile_features(PA2_SRC PRIVATE cxx_range_for)

################################################################################
# Tests
#add_executable(scene_unittest tests/scene_unittest.cpp)
#target_link_libraries(scene_unittest gtest_main PA2_SRC)

################################################################################
# Executables
#
add_executable(rasterizer main.cpp)
target_link_libraries(rasterizer PA2_SRC)
target_compile_features(rasterizer PRIVATE cxx_range_for)
