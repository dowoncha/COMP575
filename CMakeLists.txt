cmake_minimum_required(VERSION 3.2)

project(RayTracer)

set(RayTracer_VERSION_MAJOR 1)
set(RayTracer_VERSION_MAJOR 0)

# Add GL packages
find_package(OpenGL REQUIRED)
if (NOT OPENGL_FOUND)
  message("ERROR: OpenGL not found")
endif(NOT OPENGL_FOUND)
set(GL_LIBRARY GL GLU X11)

include_directories(src)

add_subdirectory(src)
add_subdirectory(apps)
