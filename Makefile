#compiler
GXX=clang++ -std=c++11 #-std=libc++

INCLUDE_PATHS = -I/usr/local/include -I/opt/X11/include -Iinclude

LIBRARY_PATHS = -L/usr/local/lib -I/opt/X11/lib

COMPILER_FLAGS = -w

LINKER_FLAGS = -lglut

SRC = $(shell find . -name *.cpp)

OBJ = $(SRC:%.cpp=%.o)

BIN = bin/raytracer

all: $(BIN)

$(BIN): $(OBJ)
		$(GXX) $(OBJ) $(INCLUDE_PATHS) $(LIBRARY_PATHS) $(COMPILER_FLAGS) $(LINKER_FLAGS)
%.o: %.cpp
	$(GXX) -c $< -o $@

clean:
	rm */*.o a.out *.ppm
