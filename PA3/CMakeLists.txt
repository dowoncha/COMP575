cmake_minimum_required(VERSION 3.2)
project(Rasterizer)

add_definitions("-std=c++11")

#########################################################
# FIND GLUT
#########################################################
find_package(GLUT REQUIRED)
include_directories(${GLUT_INCLUDE_DIRS})
link_directories(${GLUT_LIBRARY_DIRS})
add_definitions(${GLUT_DEFINITIONS})
if(NOT GLUT_FOUND)
    message(ERROR " GLUT not found!")
endif(NOT GLUT_FOUND)
#########################################################
# FIND OPENGL
#########################################################
find_package(OpenGL REQUIRED)
include_directories(${OpenGL_INCLUDE_DIRS})
link_directories(${OpenGL_LIBRARY_DIRS})
add_definitions(${OpenGL_DEFINITIONS})
if(NOT OPENGL_FOUND)
    message(ERROR " OPENGL not found!")
endif(NOT OPENGL_FOUND)

# find glew
INCLUDE(FindGLEW REQUIRED)
include_directories(${GLEW_INCLUDE_DIR})

########################################################
# Linking & stuff
#########################################################
include_directories(lib/glm)
include_directories(src)

#add_library(RAST_LIBRARY glew freeglut)

add_executable(rasterizer src/main.cpp)
target_link_libraries(rasterizer ${OPENGL_LIBRARIES} ${GLUT_LIBRARY} ${GLEW_LIBRARIES})
#target_link_libraries(rasterizer RAST_LIBRARY)
