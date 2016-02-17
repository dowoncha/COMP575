add_executable (image3 image3.cpp)
add_executable (gltracer gltracer.cpp)

target_link_libraries(image3 project_SRCS)
target_link_libraries(gltracer project_SRCS
                               ${CMAKE_SOURCE_DIR}/lib/libgl3w.a
                               ${CMAKE_SOURCE_DIR}/lib/libglut.a)

target_compile_features( image3 PRIVATE cxx_range_for)
target_compile_features( gltracer PRIVATE cxx_range_for)

#add_definitions(-DNOMINMAX)