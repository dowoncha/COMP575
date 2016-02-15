This ray tracer currently requires glut to run although I don't use right now.
Also required the clang compiler, but this can be changed to g++ in the Makefile.

make - to build objects and it makes ./a.out file, run a.out file to generate
3 images.

images generated are no-aa.ppm 8x8random, 8x8uniform.
Everything also outputs to a log file in logs.

Estimated run time is 22 seconds to generate the 3 images.

make clean - delete all object files, executable, and image files.
