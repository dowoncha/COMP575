Tested on unix, make sure all neccessary gl dependencies are installed.

Freeglut - provided
glew - provided
glm - provided

Move the lighting from default position using keyboard
wasd - x, y axis movement
qe - z axis movement
t - Switch between immediate and buffer render modes

Around 250 fps for immediate mode, 320 fps for buffer mode

Installation
-----------------------------------------
mkdir build
cd build
cmake .. -DFREEGLUT_BUILD_DEMOS=OFF
make
cp ../src/bunny.obj ./
./rasterizer
