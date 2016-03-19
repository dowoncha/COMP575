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

include_directories(lib/glfw/include)
include_directories(lib/gl3w/include)
include_directories(lib/glm)
include_directories(${GTEST_ROOT}/include)

add_subdirectory(lib/glfw)
add_subdirectory(lib/gl3w)
add_subdirectory(tools/googletest)

include_directories(src)
add_library(RAST_LIBRARY src/Renderer.cpp
                    src/Scene.cpp
                    src/Image.cpp
                    src/Light.hpp
                    src/Utility.hpp
           )

target_link_libraries(RAST_LIBRARY glfw ${GLFW_LIBRARIES})
target_compile_features(RAST_LIBRARY PRIVATE cxx_range_for)

################################################################################
# Tests
################################################################################
#add_executable(scene_unittest tests/scene_unittest.cpp)
#target_link_libraries(scene_unittest gtest_main PA2_SRC)

################################################################################
# Executables
################################################################################
file (GLOB_RECURSE EXECUTABLES ${PROJECT_SOURCE_DIR}/apps/*.cpp)
foreach(program ${EXECUTABLE})
  message(STATUS "Processing file: ${program}")
  get_filename_component(programname ${program} NAME_WE )
  add_executable(${programname} ${program})
  target_link_libraries(${programname} RAST_LIBRARY)
endforeach(program ${EXECTABLES})