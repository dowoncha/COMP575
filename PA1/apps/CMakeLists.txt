add_executable (image3 image3.cpp)
#add_executable (gltracer gltracer.cpp)

# FreeGLUT
#include(FindPkgConfig)
#pkg_check_modules(freeglut REQUIRED freeglut-gles>=3.0.0)
#if(freeglut_FOUND)
#  include_directories(${freeglut_STATIC_INCLUDE_DIRS})
#  link_directories(${freeglut_STATIC_LIBRARY_DIRS})
#  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${freeglut_STATIC_CFLAGS_OTHER}")
#  add_definitions(${freeglut_STATIC_CFLAGS_OTHER})
#endif()

#target_link_libraries(gltracer gl3w ${freeglut_STATIC_LIBRARIES})

target_link_libraries(image3 project_SRCS)
#target_link_libraries(gltracer project_SRCS)

target_compile_features( image3 PRIVATE cxx_range_for)
#target_compile_features( gltracer PRIVATE cxx_range_for)

#add_definitions(-DNOMINMAX)
