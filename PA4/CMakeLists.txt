cmake_minimum_required(VERSION 2.8)
project(RayTracer)

add_definitions("-w -std=c++14")

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

# linkand include freeglut
include_directories(lib/freeglut/include)
add_subdirectory(lib/freeglut)
include_directories(lib/glew/include)
include_directories(lib/eigen)

include_directories(src)

set(SRCS src/ray_tracer.cpp
         src/scene.cpp
         src/primitives/camera.cpp
       src/primitives/surface_kd_mesh.cpp)

add_executable(sibenik apps/sibenik.cpp ${SRCS})
target_link_libraries(sibenik ${OPENGL_LIBRARIES}
              ${CMAKE_SOURCE_DIR}/lib/freeglut/lib/libglut.so
              ${CMAKE_SOURCE_DIR}/lib/glew/lib/libGLEW.so
            -lstdc++ -lm)

add_executable(app-spheres apps/app-spheres.cpp ${SRCS})
target_link_libraries(app-spheres ${OPENGL_LIBRARIES}
              ${CMAKE_SOURCE_DIR}/lib/freeglut/lib/libglut.so
              ${CMAKE_SOURCE_DIR}/lib/glew/lib/libGLEW.so
            -lstdc++ -lm)
