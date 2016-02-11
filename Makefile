#compiler
GXX=g++ -std=c++11 #-std=libc++

SRC = $(shell find . -name *.cpp)

OBJ = $(SRC:%.cpp=%.o)

BIN = bin/raytracer

all: $(BIN)

$(BIN): $(OBJ)
		$(GXX) $(OBJ)

%.o: %.cpp
	$(GXX) -c $< -o $@

clean:
	rm a.out
	rm */*.o
