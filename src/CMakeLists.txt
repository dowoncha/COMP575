set(HEADER_FILES Camera.h
                 Image.h
                 Light.h
                 Material.h
                 Primitives.h
                 Ray.h
                 RayTracer.h
                 Scene.h
                 Surface.h
                 Utility.h)

add_library(project_SRCS Camera.cpp
                         Image.cpp
                         Light.cpp
                         Material.cpp
                         RayTracer.cpp
                         Scene.cpp
                         Surface.cpp
                         ${HEADER_FILES})

target_include_directories(project_SRCS PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_compile_features(project_SRCS PRIVATE cxx_range_for)
