This ray tracer currently requires glut to run although I don't use right now.
Also required the clang compiler, but this can be changed to g++ in the Makefile.

make - to build objects and it makes ./a.out file, run a.out file to generate
4 images.

images generated are bw.ppm, no-aa.ppm, 8x8uniform, 8x8random
Also outputs to a log file in logs folder.

make clean - delete all object files, executable, and image files.

bw.ppm is assignment part 1, generates a black circle in the image.
no-aa.ppm is quick, no anti-aliasing est 2 second run time..

Both 8x8uniform and 8x8random take approximately 22 seconds.
64 samples per pixel, uniform sampling and random sampling.

Total run time is approximately 53 seconds to generate all 4 images.

Mirror reflections are currently disabled.
