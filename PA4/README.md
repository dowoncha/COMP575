INSTALLATION:
use cmake
requires a c++ with c++14 features. clang-3.8, gcc-5, vs2015,

mkdir build
cd build
cmake .. -DCMAKE_CXX_COMPILER=cxx -DFREEGLUT_BUILD_DEMOS=off

./app-spheres
./sibenikshadows

