cmake_minimum_required(VERSION 3.2)
project(RayTracer)

add_definitions("-std=c++11")

#########################################################
# FIND OPENGL
#########################################################
find_package(OpenGL REQUIRED)
include_directories(${OpenGL_INCLUDE_DIRS})
link_directories(${OpenGL_LIBRARY_DIRS})
add_definitions(${OpenGL_DEFINITIONS})
if(NOT OPENGL_FOUND)
    message(ERROR "OPENGL not found!")
endif(NOT OPENGL_FOUND)

# link freeglut, glew, glm
include_directories(lib/freeglut/include)
add_subdirectory(lib/freeglut)
include_directories(lib/glew/include)
include_directories(lib/glm)

include_directories(src)

add_executable(rasterizer src/main.cpp)
target_link_libraries(rasterizer ${OPENGL_LIBRARIES}
                                 ${CMAKE_SOURCE_DIR}/lib/freeglut/lib/libglut.so
                                 ${CMAKE_SOURCE_DIR}/lib/glew/lib/libGLEW.so)
